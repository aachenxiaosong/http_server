/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : uni_http.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2018.06.20
 *
 **************************************************************************/

#include "uni_http.h"

#include "curl/curl.h"
#include "uni_log.h"

#define TAG "http"

typedef struct {
  char   **result;
  size_t len;
} GetCallbackParam;

static size_t _get_write_data(void *buf, size_t size, size_t nmemb,
                              void *userp) {
  size_t bytes = nmemb * size;
  GetCallbackParam *param = (GetCallbackParam *)userp;
  char *result = *(param->result);
  size_t len = param->len;
  char *temp;
  temp = (char *)uni_malloc(bytes + len + 1);
  uni_memset(temp, 0, bytes + len + 1);
  if (len > 0) {
    uni_memcpy(temp, result, len);
    uni_free(result);
  }
  uni_memcpy(temp + len, buf, bytes);
  *(param->result) = temp;
  param->len += bytes;
  return bytes;
}

static struct curl_slist *_load_headers(const char *headers[][2], int header_num) {
  struct curl_slist *header_list = NULL;
  char header[128] = {0};
  int i = 0;
  while (header_num > i) {
    uni_snprintf(header, sizeof(header), "%s: %s", headers[i][0], headers[i][1]);
    header_list = curl_slist_append(header_list, header);
    i++;
  }
  return header_list;
}

int HttpGetWithHeadersTimeout(const char *url, const char *headers[][2],
                              int header_num, uni_u32 timeout,
                              char **result) {
  int ret = -1;
  CURLcode res;
  CURL *curl;
  GetCallbackParam param;
  struct curl_slist *header_list = NULL;
  curl = curl_easy_init();
  if (NULL == curl) {
    return ret;
  }
  curl_easy_setopt(curl, CURLOPT_URL, url);
  if (result != NULL) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _get_write_data);
    param.result = result;
    param.len = 0;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&param);
  }
  header_list = _load_headers(headers, header_num);
  if (NULL != header_list) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
  }
  if (timeout > 0) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  }
  res = curl_easy_perform(curl);
  if (0 == res) {
    ret = 0;
  }
  curl_slist_free_all(header_list);
  curl_easy_cleanup(curl);
  return ret;
}

int HttpPostWithHeadersTimeout(const char *url, const char *content,
                               const char *headers[][2], int header_num,
                               uni_u32 timeout, char **result) {
  int ret = -1;
  CURLcode res;
  CURL *curl;
  GetCallbackParam param;
  struct curl_slist *header_list = NULL;
  curl = curl_easy_init();
  if (NULL == curl) {
    return ret;
  }
  curl_easy_setopt(curl, CURLOPT_URL, url);
  if (result != NULL) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _get_write_data);
    param.result = result;
    param.len = 0;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&param);
  }
  header_list = _load_headers(headers, header_num);
  if (NULL != header_list) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
  }
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);
  if (timeout > 0) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  }
  res = curl_easy_perform(curl);
  if (0 == res) {
    ret = 0;
  }
  curl_slist_free_all(header_list);
  curl_easy_cleanup(curl);
  return ret;
}

#define urlencode_isvalid(c) \
( \
  (((c) >= 'a') && ((c) <= 'z')) || \
  (((c) >= 'A') && ((c) <= 'Z')) || \
  (((c) >= '0') && ((c) <= '9')) || \
  ((c) == ':') || \
  ((c) == '&') || \
  ((c) == '/') || \
  ((c) == '=') || \
  ((c) == '?') || \
  ((c) == '.') || \
  ((c) == '-') || \
  ((c) == '_') || \
  ((c) == '~') \
)

static const char g_urlencode_hex_table[] = "0123456789ABCDEF";

static int _urlencode_strlen(const char *p) {
  int len = 0;
  if (p == NULL) {
    return 0;
  }
  for (; '\0' != *p; p++, len++) {
    if (!urlencode_isvalid(*p)) {
      len += 2;
    }
  }
  return len;
}

static char *_urlencode_strcpy(char *buf, const char *str) {
  char *q;
  const char *p;
  if (str == NULL) {
    *buf = 0;
    return buf;
  }
  for (p = str, q = buf; *p; p++, q++) {
    if (urlencode_isvalid(*p)) {
      *q = *p;
    }else {
      if (*p == ' ') {
        *q = '+';
      }
      else {
        *q++ = '%';
        *q++ = g_urlencode_hex_table[(*p >> 4) & 15];
        *q = g_urlencode_hex_table[*p & 15];
      }
    }
  }
  *q = 0;
  return q;
}

char *HttpUrlEncode(const char *str) {
  char *res;
  res = uni_malloc(_urlencode_strlen(str) + 1);
  if (res == NULL) {
    return NULL;
  }
  _urlencode_strcpy(res, str);
  return res;
}

int HttpsGetWithHeadersTimeout(const char *url, const char *capath,
                               const char *headers[][2], int header_num,
                               uni_u32 timeout, char **result) {
  int ret = -1;
  CURLcode res;
  CURL *curl;
  GetCallbackParam param;
  struct curl_slist *header_list = NULL;
  curl = curl_easy_init();
  if (NULL == curl) {
    return ret;
  }
  curl_easy_setopt(curl, CURLOPT_URL, url);
  if (result != NULL) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _get_write_data);
    param.result = result;
    param.len = 0;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&param);
  }
  header_list = _load_headers(headers, header_num);
  if (NULL != header_list) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
  }
#if UNI_HTTPS_ENABLE
  if(NULL == capath) {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  } else {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, capath); //CURLOPT_CAPATH
  }
#endif
  if (timeout > 0) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  }
  res = curl_easy_perform(curl);
  if (0 == res) {
    ret = 0;
  }
  curl_slist_free_all(header_list);
  curl_easy_cleanup(curl);
  return ret;
}

int HttpsPostWithHeadersTimeout(const char *url, const char *capath, const char *content,
                               const char *headers[][2], int header_num,
                               uni_u32 timeout, char **result) {
  int ret = -1;
  CURLcode res;
  CURL *curl;
  GetCallbackParam param;
  struct curl_slist *header_list = NULL;
  curl = curl_easy_init();
  if (NULL == curl) {
    return ret;
  }
  curl_easy_setopt(curl, CURLOPT_URL, url);
  if (result != NULL) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _get_write_data);
    param.result = result;
    param.len = 0;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&param);
  }
  header_list = _load_headers(headers, header_num);
  if (NULL != header_list) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
  }
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);

#if UNI_HTTPS_ENABLE
  if(NULL == capath) {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  } else {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, capath);
  }
#endif

  if (timeout > 0) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  }
  res = curl_easy_perform(curl);
  if (0 == res) {
    ret = 0;
  }
  curl_slist_free_all(header_list);
  curl_easy_cleanup(curl);
  return ret;
}

static void _url_parse_param(char *param, cJSON *param_dict) {
  char *head = param;
  char *index = NULL;
  if (0 == uni_strlen(head)) return;
  if (NULL == (index = uni_strchr(head, '='))) {
    cJSON_AddStringToObject(param_dict, head, "");
    return;
  }
  *index = '\0';
  cJSON_AddStringToObject(param_dict, head, index + 1);
  return;
}

static void _url_parse_params(char *params, cJSON *out_dict) {
  char *head = params;
  char *index = NULL;
  cJSON *param_dict = cJSON_CreateObject();
  while (*head != '\0') {
    if (NULL == (index = uni_strchr(head, '&'))) {
      _url_parse_param(head, param_dict);
      break;
    }
    *index = '\0';
    _url_parse_param(head, param_dict);
    head = index + 1;
  }
  cJSON_AddItemToObject(out_dict, "params", param_dict);
  return;
}

int HttpUrlParse(const char *url, cJSON **out_dict) {
  char *url_buffer = NULL;
  uni_s32 url_buffer_len = 0;
  char *index = NULL;
  if (NULL == url || 0 == uni_strlen(url) || NULL == out_dict) {
    LOGE(TAG, "invalid params");
    return -1;
  }
  url_buffer_len = uni_strlen(url);
  url_buffer = (char *)uni_malloc(url_buffer_len + 1);
  if (!url_buffer) {
    LOGE(TAG, "invalid params");
    return -1;
  }
  uni_memset(url_buffer, 0, url_buffer_len + 1);
  uni_strncpy(url_buffer, url, url_buffer_len);
  *out_dict = cJSON_CreateObject();
  index = uni_strchr(url_buffer, '?');
  if (NULL == index) {
    cJSON_AddStringToObject(*out_dict, "path", url_buffer);
    goto L_END;
  }
  *index = '\0';
  cJSON_AddStringToObject(*out_dict, "path", url_buffer);
  _url_parse_params(index + 1, *out_dict);
L_END:
  uni_free(url_buffer);
  return 0;
}
