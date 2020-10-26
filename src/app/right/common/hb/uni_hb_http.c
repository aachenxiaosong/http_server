#include "uni_hb_http.h"
#include "uni_time_tick.h"
#include "uni_http.h"
#include "uni_log.h"
#include "uni_json.h"
#include "uni_device.h"
#include "uni_auth_http.h"
#include "configurable_info.h"

#define HB_HTTP_TAG "hb_http"
extern char * manage_get_current_page_name(void);
static int _get_token(char *token) {
  Token *t = HttpGetToken();
  if (t != NULL) {
    strcpy(token, t->token);
    HttpGetTokenFree(t);
    return 0;
  }
  LOGE(HB_HTTP_TAG, "get token failed");
  return -1;
}

static int _parse_result(cJSON *jresult) {
  int err_code = -1;

  JsonReadItemInt(jresult, "status", &err_code);
  if (0 != err_code) {
    LOGE(HB_HTTP_TAG, "hb status error: %d", err_code);
    return -1;
  }

  return 0;
}

int HttpHbReport(void) {
  int ret = -1;
  char content_post[1024] = {0};
  char *result = NULL;
  cJSON *jresult = NULL;
  char token[128];
  const char *http_headers[2][2] = {{"Content-Type",
                               "application/json;charset=UTF-8"},
                              {NULL, NULL}};
  LOGT(HB_HTTP_TAG, "elevator IPC heart beat BEGIN ------------------------------------");
  if (0 != _get_token(token)) {
    LOGT(HB_HTTP_TAG, "elevator IPC heart beat END ------------------------------------\n");
    return ret;
  }

  uni_snprintf(content_post, sizeof(content_post),
               "{"
                 "\"appKey\": \"%s\","
                 "\"deviceSn\": \"%s\","
                 "\"up_time\": %d,"
                 "\"device_type\": \"%s\","
                 "\"version_number\": \"%s\""
               "}",
               DeviceGetAppKey(),
               DeviceGetUdid(),
               uni_get_now_sec_unix(),
               MY_TYPE,
               MY_VERSION);
  LOGT(HB_HTTP_TAG, "request:%s; token:%s", content_post, token);
  
  http_headers[1][0] = "token";
  http_headers[1][1] = token;
  
  if (0 != HttpPostWithHeadersTimeout(CLOUD_HB_REPORT_URL, 
                                      content_post,
                                      (const char * (*)[2])http_headers,
                                      sizeof(http_headers) /
                                      sizeof(http_headers[0]),
                                      5, &result)) {
    LOGE(HB_HTTP_TAG, "post failed");
    LOGT(HB_HTTP_TAG, "elevator IPC heart beat END ------------------------------------\n");
    return ret;
  }
  LOGT(HB_HTTP_TAG, "parsing result:%s", result);
  jresult = cJSON_Parse(result);
  if (jresult != NULL) {
    if(_parse_result(jresult) == 0) {
       LOGT(HB_HTTP_TAG, "report hb succ");
       ret = 0;
    }
    cJSON_Delete(jresult);
  }
  
  uni_free(result);
  LOGT(HB_HTTP_TAG, "elevator IPC heart beat END ------------------------------------\n");
  return ret;
}
