#include "uni_iot.h"
#include "uni_auth.h"
#include "uni_md5sum.h"
#include "uni_bt_network.h"
#include "uni_event_list.h"
#include "uni_http.h"
#include "uni_json.h"
#include "uni_config.h"
#include "uni_log.h"
#include "uni_black_board.h"

#define  AUTH_TAG "AUTH"  

#define AUTH_DOMAIN_NAME            "http://test-route.igaicloud.cn:8088/auth"
#define UC_DOMAIN_NAME              "http://uc.hivoice.cn/timestamp.jsp"
#define CHECK_INTERVAL_SEC          (10) //for test
#define UPDATE_INTERVAL_SEC         (60) //for test
#define REGISTER_INTERVAL_SEC       (60) //for test
#define AUTH_FAIL_NUM               3


#define UNISOUND_APPKEY_LEN     (40)
#define UNISOUND_APPSECRET_LEN  (32)
#define UNISOUND_UDID_LEN       (127)
#define UNISOUND_RET_CODE_LEN   (32)
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
} g_signature_param;

static struct {
  AuthStatus      auth_status;
  unsigned int    update_tick_cnt;
  unsigned int    check_tick_cnt;
  unsigned int    register_tick_cnt;
  int             valid_time;
  int             update_token_time;
  uni_pthread_t   auth_tsk_pid;
  uni_mutex_t     auth_lock;
  char            config_path[FILE_PATH_LENGTH];
  int             is_valid;
  int             is_running;
} g_auth;


static void _get_timestamp_from_uc(char *timestamp, int size) {
  char *result = NULL;
  int ret = HttpGet(UC_DOMAIN_NAME, &result);
  if (ret != 0) {
    LOGE(AUTH_TAG, "get timestamp from uc failed\n");
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
    free(result);
  }
  LOGT(AUTH_TAG, "get timestamp %s\n", timestamp);
}
#if 0
static void _str_sort(const char *strs[], int start, int end) {
  int i = start, j = end;
  const char *base = strs[i];
  if (start >= end) {
    return;
  }
  while (i < j) {
    while (i < j && strcmp(strs[j], base) >= 0) {
      j--;
    }
    strs[i] = strs[j];
    while (i < j && strcmp(strs[i], base) <= 0) {
      i++;
    }
    strs[j] = strs[i];
  }
  strs[i] = base;
  _str_sort(strs, start, i - 1);
  _str_sort(strs, j + 1, end);
}

static char *_str_cat(const char *strs[], int str_num) {
  int i;
  int total_len = 0;
  char *cat_str;
  for (i = 0; i < str_num; i++) {
    total_len += strlen(strs[i]);
  }
  cat_str = (char *)malloc(total_len + 1);
  if (NULL == cat_str) {
    LOG_ERR("memory malloc failed\n");
    return NULL;
  }
  *cat_str = '\0';
  for (i = 0; i < str_num; i++) {
    strcat(cat_str, strs[i]);
  }
  return cat_str;
}
#endif
static void _byte2hex(uint8_t* in, char* out, int in_len) {
  char* dict = "0123456789abcdef";
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
  
  LOGT(AUTH_TAG, "_build_str:%s", in_str);
  memset(digest, 0, 16);
  Md5sum((const uint8_t *)in_str, strlen((char *)in_str), digest);
  _byte2hex(digest, result, 16);

  LOGT(AUTH_TAG, "_build_signature:%s", result);
  return 0;
}

static int _set_params(const char *appkey, const char *appsecret,
                       const char *udid) {
  if (NULL == appkey || NULL == appsecret || NULL == udid) {
    return -1;
  }
  snprintf(g_signature_param.appkey, sizeof(g_signature_param.appkey), "%s", appkey);
  snprintf(g_signature_param.appsecret, sizeof(g_signature_param.appsecret), "%s", appsecret);
  snprintf(g_signature_param.udid, sizeof(g_signature_param.udid), "%s", udid);
  return 0;
}

static int _analysis_register_result(char *result_str, char *ret_code, int len,
                                     int *valid_time) {
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
  snprintf(ret_code, len, "%s", returnCode->valuestring);

  
  result = cJSON_GetObjectItem(root, CJSON_OBJECT_NAME(data));
  if (NULL == result) {
    goto L_ERROR;
  }

  validTime = cJSON_GetObjectItem(result, CJSON_OBJECT_NAME(expire));
  if (NULL == validTime) {
    goto L_ERROR;
  }
  *valid_time = validTime->valueint;
  token = cJSON_GetObjectItem(result, CJSON_OBJECT_NAME(accessToken));
  if (NULL == token || NULL == token->valuestring) {
    goto L_ERROR;
  }
  snprintf(g_signature_param.token, sizeof(g_signature_param.token), "%s", token->valuestring);
  //_write_token_to_config();
  cJSON_Delete(root);
  return 0;
L_ERROR:
  cJSON_Delete(root);
  return -1;
}

static int _auth_register_token(char *ret_code, int ret_code_len, int *valid_time) {
  int ret;
  char *result = NULL;
  char msg[512] = {0};
  
  const char *headers[1][2] = {{"Content-Type", "application/json;charset=UTF-8"}};
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
  LOGT(AUTH_TAG, "post msg:%s\n", msg);
  ret = HttpPostWithHeadersTimeout(AUTH_DOMAIN_NAME"/token", msg, headers, 1, 10, &result);
  LOGT(AUTH_TAG, "result:%s\n", result);
  LOGT(AUTH_TAG, "ret=%d\n", ret);
  if (0 == ret &&
      NULL != result &&
      0 == _analysis_register_result(result, ret_code, ret_code_len,
                                     valid_time)) {
    LOGT(AUTH_TAG, "auth register succeeded: return value:%d code:%s\n",
        ret, ret_code);
    ret = 0;
  } else {
    LOGE(AUTH_TAG, "auth register failed: return value:%d code:%s\n",
        ret, ret_code);
    ret = -1;
  }
  if (NULL != result) {
    free(result);
  }
  return ret;
}
static int _auth_register() {
  char ret_code[UNISOUND_RET_CODE_LEN + 1] = {0};
  int ret = -1;

  _auth_register_token(ret_code, sizeof(ret_code),
               &g_auth.valid_time);
  if (0 == strcmp(ret_code, "OK")) {
    ret = 0;
  }
  if (0 == strlen(ret_code)) {
    snprintf(ret_code, sizeof(ret_code), "%s", "AUTH_FAILED");
  }

  return ret;
}

const char* _status2string(AuthStatus status) {
  switch (status) {
    case AUTH_IDLE:
      return "AUTH_IDLE";
    case AUTH_REGISTER:
      return "AUTH_REGISTER";
    case AUTH_UPDATE:
      return "AUTH_UPDATE";
    case AUTH_CHECK:
      return "AUTH_CHECK";
    default:
      return "INVALID_STATE";
  }
}

void _reset_auth_tick_cnt(AuthStatus status) {
  switch (status) {
    case AUTH_IDLE:
      break;
    case AUTH_REGISTER:
      g_auth.register_tick_cnt = 0;
      break;
    case AUTH_UPDATE:
      g_auth.update_tick_cnt = 0;
      break;
    case AUTH_CHECK:
      g_auth.check_tick_cnt = 0;
      break;
    default:
      break;
  }
}

void _increase_auth_tick_cnt(AuthStatus status) {
  switch (status) {
    case AUTH_IDLE:
      break;
    case AUTH_REGISTER:
      g_auth.register_tick_cnt++;
      break;
    case AUTH_UPDATE:
      g_auth.update_tick_cnt++;
      break;
    case AUTH_CHECK:
      g_auth.check_tick_cnt++;
      break;
    default:
      break;
  }
}

int _check_auth_tick_cnt(AuthStatus status) {
  switch (status) {
    case AUTH_IDLE:
      return 0;
    case AUTH_REGISTER:
      return (0 == (g_auth.register_tick_cnt % REGISTER_INTERVAL_SEC));
    case AUTH_UPDATE:
      return (0 == (g_auth.update_tick_cnt % g_auth.update_token_time));
    case AUTH_CHECK:
      return (0 == (g_auth.check_tick_cnt % CHECK_INTERVAL_SEC));
    default:
      return 0;
  }
}

void _set_auth_status(AuthStatus status) {
  //_status2string(g_auth.auth_status), _status2string(status));
  _reset_auth_tick_cnt(status);
  g_auth.auth_status = status;
}

static void* _auth_tsk(void *args) {
  unsigned int auth_fail_cnt = 0;

  LOGT(AUTH_TAG, "@@@@@@@@@@@@@@@@@@_auth_tsk start");
  while(g_auth.is_running) {
    uni_pthread_mutex_lock(&g_auth.auth_lock);
    switch (g_auth.auth_status) {
      case AUTH_IDLE:
        break;
      case AUTH_REGISTER:
        if (_check_auth_tick_cnt(AUTH_REGISTER)) {
          if (0 == _auth_register()) {
            g_auth.update_token_time = g_auth.valid_time / 2;
            LOGT(AUTH_TAG, "g_auth.valid_time:%d, update_token_time:%d.\n",
                    (int)g_auth.valid_time, (int)g_auth.update_token_time);
            g_auth.is_valid = 1;
            auth_fail_cnt = 0;
            _set_auth_status(AUTH_CHECK);
          } else {
            auth_fail_cnt++;
            LOGT(AUTH_TAG, "auth failed for %d tries\n", auth_fail_cnt);
            if (auth_fail_cnt >= AUTH_FAIL_NUM) {
              LOGT(AUTH_TAG, "unisound service disabled!\n");
              g_auth.is_valid = 0;
            }
          }
        }
        break;
      case AUTH_CHECK:
        if (_check_auth_tick_cnt(AUTH_UPDATE)) {
          g_auth.register_tick_cnt = REGISTER_INTERVAL_SEC;
          _set_auth_status(AUTH_REGISTER);
          uni_pthread_mutex_unlock(&g_auth.auth_lock);
          continue;
        }
        break;
      default:
        LOGT(AUTH_TAG, "wrong state!\n");
        break;
    }
    _increase_auth_tick_cnt(AUTH_REGISTER);
    _increase_auth_tick_cnt(AUTH_UPDATE);
    _increase_auth_tick_cnt(AUTH_CHECK);
    uni_pthread_mutex_unlock(&g_auth.auth_lock);
    uni_sleep(1);
  }
  return NULL;
}

int AuthGetToken(char *token)
{
  int ret = -1;  
  uni_pthread_mutex_lock(&g_auth.auth_lock);
  if(g_auth.is_valid == 1) {
    memcpy(token, g_signature_param.token, strlen(g_signature_param.token));
    ret = 0;
  }
  uni_pthread_mutex_unlock(&g_auth.auth_lock);

  return ret;
}


int AuthInit(const char *config_path, const char *appkey, const char *secret, const char *udid) {
  extern char* DeviceGetUdid(void);
  struct thread_param tpara;
  uni_memset(&tpara, 0, sizeof(struct thread_param));
  tpara.stack_size = STACK_DEFAULT_SIZE;
  tpara.th_priority = OS_PRIORITY_NORMAL;
  uni_strncpy(tpara.task_name, "AUTH", sizeof(tpara.task_name) - 1);
  
  memset(&g_auth, 0, sizeof(g_auth));

  LOGD(AUTH_TAG, "@@@@@@@@@@@@@@@@@@AuthInit");
  if (NULL != config_path) {
    sprintf(g_auth.config_path, "%s/token_file", config_path);
  } else {
    sprintf(g_auth.config_path, "config/token_file");
  }
  g_auth.is_valid = 0;
  g_auth.is_running = 1;
  g_auth.update_token_time = UPDATE_INTERVAL_SEC;
  g_auth.register_tick_cnt = REGISTER_INTERVAL_SEC - 10;

  _set_params(appkey, secret, udid);
  uni_pthread_mutex_init(&g_auth.auth_lock);
  //_set_auth_status(AUTH_IDLE);
  _set_auth_status(AUTH_REGISTER);
  if (0 != uni_pthread_create(&g_auth.auth_tsk_pid, &tpara, _auth_tsk, NULL)) {
    LOGE(AUTH_TAG, "pthread_create failed, errno=%d", errno);
    uni_pthread_mutex_destroy(&g_auth.auth_lock);
    return -1;
  }
  return 0;
}

void AuthFinit(void) {
  g_auth.is_running = 0;
  //uni_pthread_join(g_auth.auth_tsk_pid, NULL);
  uni_pthread_mutex_destroy(&g_auth.auth_lock);
}


