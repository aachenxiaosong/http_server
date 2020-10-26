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
 * Description : uni_mc_param.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.03.31
 *
 **************************************************************************/

#include "uni_mc_param.h"
#include "uni_sha.h"
#include "uni_time_tick.h"
#include "uni_log.h"

#define MC_PARAM_TAG "mc_param"

static Result _set_subsystem_id(RegisterParam *param, const char *subsystem_id) {
  uni_s32 len = uni_strlen(subsystem_id);
  if (len > SUBSYSTEM_ID_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->subsystem_id, subsystem_id);
  return E_OK;
}

static Result _set_data_version(RegisterParam *param, const char *data_version) {
  uni_s32 len = uni_strlen(data_version);
  if (len > DATA_VERSION_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->data_version, data_version);
  return E_OK;
}

static Result _set_appkey(RegisterParam *param, const char *appkey) {
  uni_s32 len = uni_strlen(appkey);
  if (len != APPKEY_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->appkey, appkey);
  return E_OK;
}

static Result _set_secretkey(RegisterParam *param, const char *secretkey) {
  uni_s32 len = uni_strlen(secretkey);
  if (len != SECRETKEY_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->secretkey, secretkey);
  return E_OK;
}

static Result _set_tc_device_id(RegisterParam *param, const char *tc_device_id) {
  uni_s32 len = uni_strlen(tc_device_id);
  if (len > TC_DEVICE_ID_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->tc_device_id, tc_device_id);
  return E_OK;
}

static Result _set_udid(RegisterParam *param, const char *udid) {
  uni_s32 len = uni_strlen(udid);
  if (len > UDID_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->udid, udid);
  return E_OK;
}

static Result _set_token(RegisterParam *param, const char *token) {
  uni_s32 len = uni_strlen(token);
  if (len > TOKEN_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->token, token);
  return E_OK;
}

static Result _set_app_os_type(RegisterParam *param, const char *app_os_type) {
  uni_s32 len = uni_strlen(app_os_type);
  if (len > TOKEN_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->app_os_type, app_os_type);
  return E_OK;
}

static Result _set_extras(RegisterParam *param, const char *extras) {
  uni_s32 len = uni_strlen(extras);
  if (NULL != param->extras) {
    uni_free(param->extras);
  }
  param->extras = (char *)uni_malloc(len + 1);
  if (NULL == param->extras) {
    LOGE(MC_PARAM_TAG, "memory malloc failed");
    return E_FAILED;
  }
  uni_strcpy(param->extras, extras);
  return E_OK;
}

Result RegisterParamSet(RegisterParam *param, RegisterParamType param_type, const char *value) {
  Result ret = E_FAILED;
  if (value == NULL) {
    LOGE(MC_PARAM_TAG, "value is NULL");
    return ret;
  }
  uni_pthread_mutex_lock(param->mutex);
  switch (param_type) {
    case REGISTER_PARAM_SUBSYSTEM_ID:
      ret = _set_subsystem_id(param, value);
      break;
    case REGISTER_PARAM_DATA_VERSION:
      ret = _set_data_version(param, value);
      break;
    case REGISTER_PARAM_APPKEY:
      ret = _set_appkey(param, value);
      break;
    case REGISTER_PARAM_SECRETKEY:
      ret = _set_secretkey(param, value);
      break;
    case REGISTER_PARAM_TC_DEVICE_ID:
      ret = _set_tc_device_id(param, value);
      break;
    case REGISTER_PARAM_UDID:
      ret = _set_udid(param, value);
      break;
    case REGISTER_PARAM_TOKEN:
      ret = _set_token(param, value);
      break;
    case REGISTER_PARAM_APP_OS_TYPE:
      ret = _set_app_os_type(param, value);
      break;
    case REGISTER_PARAM_EXTRAS:
      ret = _set_extras(param, value);
      break;
    default:
      LOGE(MC_PARAM_TAG, "wrong param type");
      break;
  }
  uni_pthread_mutex_unlock(param->mutex);
  return ret;
}

char* RegisterParamGet(RegisterParam *param, RegisterParamType param_type) {
  char *ret = NULL;
  switch (param_type) {
    case REGISTER_PARAM_SUBSYSTEM_ID:
      ret = param->subsystem_id;
      break;
    case REGISTER_PARAM_DATA_VERSION:
      ret = param->data_version;
      break;
    case REGISTER_PARAM_APPKEY:
      ret = param->appkey;
      break;
    case REGISTER_PARAM_SECRETKEY:
      ret = param->secretkey;
      break;
    case REGISTER_PARAM_TC_DEVICE_ID:
      ret = param->tc_device_id;
      break;
    case REGISTER_PARAM_UDID:
      ret = param->udid;
      break;
    case REGISTER_PARAM_TOKEN:
      ret = param->token;
      break;
    case REGISTER_PARAM_APP_OS_TYPE:
      ret = param->app_os_type;
      break;
    case REGISTER_PARAM_EXTRAS:
      ret = param->extras;
      break;
    default:
      LOGE(MC_PARAM_TAG, "wrong param type");
      break;
  }
  return ret;
}

void RegisterParamPrint(RegisterParam *param) {
  printf("Register Param:"
         "subsystem_id:%s\n"
         "data_version:%s\n"
         "appkey:%s\n"
         "secretkey:%s\n"
         "tc_device_id:%s\n"
         "udid:%s\n"
         "token:%s\n"
         "app_os_type:%s\n"
         "extras:%s\n",
         param->subsystem_id,
         param->data_version,
         param->appkey,
         param->secretkey,
         param->tc_device_id,
         param->udid,
         param->token,
         param->app_os_type,
         param->extras);
}

Result RegisterParamInit(RegisterParam *param) {
  uni_memset(param, 0, sizeof(RegisterParam));
  uni_pthread_mutex_init(&param->mutex);
  return E_OK;
}

void RegisterParamFinal(RegisterParam *param) {
  uni_pthread_mutex_destroy(param->mutex);
  if (NULL != param->extras) {
    uni_free(param->extras);
  }
}

static void _str_sort(const char *strs[], uni_s32 start, uni_s32 end) {
  uni_s32 i = start, j = end;
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

static char *_str_cat(const char *strs[], uni_s32 str_num) {
  uni_s32 i;
  uni_s32 total_len = 0;
  char *cat_str;
  for (i = 0; i < str_num; i++) {
    total_len += uni_strlen(strs[i]);
  }
  cat_str = (char *)uni_malloc(total_len + 1);
  if (NULL == cat_str) {
    LOGE(MC_PARAM_TAG, "memory malloc failed");
    return NULL;
  }
  *cat_str = '\0';
  for (i = 0; i < str_num; i++) {
    uni_strcat(cat_str, strs[i]);
  }
  return cat_str;
}

int _build_signature(const char *params[], uni_s32 param_num, char *result) {
  char *in_str = NULL;
  _str_sort(params, 0, param_num - 1);
  in_str = _str_cat(params, param_num);
  if (NULL == in_str) {
    return -1;
  }
  SHA1DigestStr(in_str, result);
  uni_free(in_str);
  return 0;
}

char* RegisterParamFormatForIgCloud(RegisterParam *param) {
  int reg_param_len;
  char *reg_param;
  const char *params[10] = {0};
  const char *appkey = RegisterParamGet(param, REGISTER_PARAM_APPKEY);
  const char *secretkey = RegisterParamGet(param, REGISTER_PARAM_SECRETKEY);
  const char *udid = RegisterParamGet(param, REGISTER_PARAM_UDID);
  const char *subsystem_id = RegisterParamGet(param, REGISTER_PARAM_SUBSYSTEM_ID);
  const char *token = RegisterParamGet(param, REGISTER_PARAM_TOKEN);
  const char *tc_device_id = RegisterParamGet(param, REGISTER_PARAM_TC_DEVICE_ID);
  char timestamp[11];
  const char *data_version = RegisterParamGet(param, REGISTER_PARAM_DATA_VERSION);
  const char *app_os_type = RegisterParamGet(param, REGISTER_PARAM_APP_OS_TYPE);
  const char *extras = RegisterParamGet(param, REGISTER_PARAM_EXTRAS);
  char signature[SIGNATURE_LEN + 1] = {0};
  uni_snprintf(timestamp, sizeof(timestamp), "%d", uni_get_now_sec_unix());
  params[0] = appkey;
  params[1] = secretkey;
  params[2] = udid;
  params[3] = subsystem_id;
  params[4] = token;
  params[5] = tc_device_id;
  params[6] = timestamp;
  params[7] = data_version;
  params[8] = app_os_type;
  params[9] = extras;
  _build_signature(params, sizeof(params) / sizeof(params[0]), signature);
  reg_param_len = uni_strlen("appKey=") + uni_strlen(params[0]) +
                  uni_strlen("&udid=") + uni_strlen(params[2]) +
                  uni_strlen("&subsystemId=") + uni_strlen(params[3]) +
                  uni_strlen("&token=") + uni_strlen(params[4]) +
                  uni_strlen("&tcDeviceId=") + uni_strlen(params[5]) +
                  uni_strlen("&timestamp=") + uni_strlen(params[6]) +
                  uni_strlen("&dataVersion=") + uni_strlen(params[7]) +
                  uni_strlen("&signature=") + uni_strlen(signature) +
                  uni_strlen("&appOsType=") + uni_strlen(params[8]) +
                  uni_strlen("&extras=") + uni_strlen(params[9]) + 1;
  reg_param = (char *)uni_malloc(reg_param_len);
  uni_sprintf(reg_param, "appKey=%s&udid=%s&subsystemId=%s&token=%s&tcDeviceId=%s"
              "&timestamp=%s&dataVersion=%s&signature=%s&appOsType=%s&extras=%s",
              appkey, udid, subsystem_id, token, tc_device_id,
              timestamp, data_version, signature, app_os_type, extras);
  return reg_param;
}
