#include "uni_auth_http.h"

#include "uni_md5sum.h"
#include "uni_http.h"
#include "uni_json.h"
#include "uni_log.h"
#include "uni_device.h"
#include "configurable_info.h"

#define  AUTH_HTTP_TAG "auth_http"  


#define UNISOUND_APPKEY_LEN     (40)
#define UNISOUND_APPSECRET_LEN  (32)
#define UNISOUND_UDID_LEN       (127)
#define UNISOUND_TOKEN_MAX_LEN  (127)
#define UNISOUND_TIMESTAMP_LEN  (10)
#define UNISOUND_SIGNATURE_LEN  (40)

#define FILE_PATH_LENGTH         256

static struct {
  char appkey[UNISOUND_APPKEY_LEN + 1];
  char appsecret[UNISOUND_APPSECRET_LEN + 1];
  char udid[UNISOUND_UDID_LEN + 1];
  char token[UNISOUND_TOKEN_MAX_LEN + 1];
  char timestamp[UNISOUND_TIMESTAMP_LEN + 1];
  char signature[UNISOUND_SIGNATURE_LEN + 1];
  int  valid_time;
} g_signature_param = {0};
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;


static void _get_timestamp_from_uc(char *timestamp, int size) {
  char *result = NULL;
  int ret = HttpGet(CLOUD_UC_TS_URL, &result);
  if (ret != 0) {
    LOGE(AUTH_HTTP_TAG, "get timestamp from uc failed\n");
    time_t timeval;
    time(&timeval);
    snprintf(timestamp, size, "%d", (unsigned int)timeval);
  } else {
    char *p = result;
    while (isspace(*p)) {
      p++;
    }
    snprintf(timestamp, size, "%s", p);
  }
  if (NULL != result) {
    uni_free(result);
  }
  LOGT(AUTH_HTTP_TAG, "get timestamp %s\n", timestamp);
}

static void _byte2hex(uint8_t* in, char* out, int in_len) {
  const char* dict = "0123456789abcdef";
  uni_s32 index = -1;
  uint8_t tmp_in;
  char* o = out;
  while (++index < in_len) {
    tmp_in = in[index];
    *o++ = dict[(tmp_in >> 4) & 0x0F];
    *o++ = dict[tmp_in & 0x0F];
  }
}

static int _build_signature(char *result) {
  char in_str[512];
  uint8_t digest[16] = {0};

  memset(in_str, 0, 512);
  snprintf(in_str, sizeof(in_str), "appKey=%s&timestamp=%s&udid=%s&secret=%s",
          g_signature_param.appkey, g_signature_param.timestamp,
          g_signature_param.udid, g_signature_param.appsecret);
  
  LOGT(AUTH_HTTP_TAG, "_build_str:%s", in_str);
  memset(digest, 0, 16);
  Md5sum((const uint8_t *)in_str, strlen((char *)in_str), digest);
  _byte2hex(digest, result, 16);

  LOGT(AUTH_HTTP_TAG, "_build_signature:%s", result);
  return 0;
}

static void _set_params() {
  snprintf(g_signature_param.appkey, sizeof(g_signature_param.appkey), "%s",
           DeviceGetAppKey());
  snprintf(g_signature_param.appsecret, sizeof(g_signature_param.appsecret), "%s",
           DeviceGetSecretKey());
  snprintf(g_signature_param.udid, sizeof(g_signature_param.udid), "%s",
           DeviceGetUdid());
}

static int _analysis_register_result(char *result_str) {
  cJSON *root = cJSON_Parse(result_str);
  cJSON *returnCode = NULL;
  cJSON *result = NULL;
  cJSON *validTime = NULL;
  cJSON *token = NULL;
  if (NULL == root) {
    return -1;
  }
  returnCode = cJSON_GetObjectItem(root, CJSON_OBJECT_NAME(message));
  if (NULL == returnCode || NULL == returnCode->valuestring){
    goto L_ERROR;
  }
  if (0 != strcmp(returnCode->valuestring, "OK")) {
    goto L_ERROR;
  }
  result = cJSON_GetObjectItem(root, CJSON_OBJECT_NAME(data));
  if (NULL == result) {
    goto L_ERROR;
  }
  validTime = cJSON_GetObjectItem(result, CJSON_OBJECT_NAME(expire));
  if (NULL == validTime) {
    goto L_ERROR;
  }
  token = cJSON_GetObjectItem(result, CJSON_OBJECT_NAME(accessToken));
  if (NULL == token || NULL == token->valuestring) {
    goto L_ERROR;
  }
  pthread_mutex_lock(&g_mutex);
  g_signature_param.valid_time = validTime->valueint;
  snprintf(g_signature_param.token, sizeof(g_signature_param.token), "%s", token->valuestring);
  pthread_mutex_unlock(&g_mutex);
  cJSON_Delete(root);
  return 0;
L_ERROR:
  cJSON_Delete(root);
  return -1;
}

static int _auth_register_token() {
  int ret;
  char *result = NULL;
  char msg[512] = {0};
  
  const char *headers[1][2] = {{"Content-Type", "application/json;charset=UTF-8"}};
  _set_params();
  _get_timestamp_from_uc(g_signature_param.timestamp,
                         sizeof(g_signature_param.timestamp));
  _build_signature(g_signature_param.signature);
  uni_snprintf(msg, sizeof(msg),
              "{"
                "\"appKey\":\"%s\","
                "\"timestamp\":\"%s\","
                "\"udid\":\"%s\","
                "\"sign\":\"%s\""
              "}",
              g_signature_param.appkey,g_signature_param.timestamp,
              g_signature_param.udid, g_signature_param.signature);
  LOGT(AUTH_HTTP_TAG, "post msg:%s\n", msg);
  ret = HttpPostWithHeadersTimeout(CLOUD_AUTH_URL, msg, headers, 1, 10, &result);
  LOGT(AUTH_HTTP_TAG, "result:%s\n", result);
  LOGT(AUTH_HTTP_TAG, "ret=%d\n", ret);
  if (0 == ret &&
      NULL != result &&
      0 == _analysis_register_result(result)) {
    LOGT(AUTH_HTTP_TAG, "auth register succeeded");
    ret = 0;
  } else {
    LOGE(AUTH_HTTP_TAG, "auth register failed");
    ret = -1;
  }
  if (NULL != result) {
    uni_free(result);
  }
  return ret;
}

Token* HttpGetToken() {
  Token *token;
  pthread_mutex_lock(&g_mutex);
  if (strlen(g_signature_param.token) > 0) {
    token = (Token *)uni_malloc(sizeof(Token));
    snprintf(token->token, sizeof(token->token), "%s", g_signature_param.token);
    token->valid_time = g_signature_param.valid_time;
    pthread_mutex_unlock(&g_mutex);
    return token;
  }
  pthread_mutex_unlock(&g_mutex);
  return NULL;
}


void HttpGetTokenFree(Token *token) {
  if (NULL != token) {
    uni_free(token);
  }
}

int HttpGetTokenCacheUpdate() {
  return _auth_register_token();
}
