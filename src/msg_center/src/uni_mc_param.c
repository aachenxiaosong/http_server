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

#include "uni_cloud_utils.h"
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

static Result _set_client_id(MqttParam *param, const char *client_id) {
  uni_s32 len = uni_strlen(client_id);
  if (len > CLIENT_ID_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->client_id, client_id);
  return E_OK;
}

static Result _set_username(MqttParam *param, const char *username) {
  uni_s32 len = uni_strlen(username);
  if (len > USERNAME_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->username, username);
  return E_OK;
}

static Result _set_password(MqttParam *param, const char *password) {
  uni_s32 len = uni_strlen(password);
  if (len > PASSWORD_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->password, password);
  return E_OK;
}

static Result _set_ip(MqttParam *param, const char *ip) {
  uni_s32 len = uni_strlen(ip);
  if (len > IP_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->ip, ip);
  return E_OK;
}

static Result _set_port(MqttParam *param, const char *port) {
  uni_s32 len = uni_strlen(port);
  if (len > PORT_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->port, port);
  return E_OK;
}

static Result _set_subscribe(MqttParam *param, const char *subscribe) {
  uni_s32 len = uni_strlen(subscribe);
  if (len > SUBSCRIBE_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->subscribe, subscribe);
  return E_OK;
}

static Result _set_subscribe1(MqttParam *param, const char *subscribe1) {
  uni_s32 len = uni_strlen(subscribe1);
  if (len > SUBSCRIBE_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->subscribe1, subscribe1);
  return E_OK;
}

static Result _set_publish(MqttParam *param, const char *publish) {
  uni_s32 len = uni_strlen(publish);
  if (len > PUBLISH_MAX_LEN) {
    LOGE(MC_PARAM_TAG, "invalid param size %d", len);
    return E_FAILED;
  }
  uni_strcpy(param->publish, publish);
  return E_OK;
}

Result MqttParamSet(MqttParam *param, MqttParamType param_type, const char *value) {
  Result ret = E_FAILED;
  if (value == NULL) {
    LOGE(MC_PARAM_TAG, "value is NULL");
    return ret;
  }
  uni_pthread_mutex_lock(param->mutex);
  switch (param_type) {
    case MQTT_PARAM_CLIENT_ID:
      ret = _set_client_id(param, value);
      break;
    case MQTT_PARAM_USERNAME:
      ret = _set_username(param, value);
      break;
    case MQTT_PARAM_PASSWORD:
      ret = _set_password(param, value);
      break;
    case MQTT_PARAM_IP:
      ret = _set_ip(param, value);
      break;
    case MQTT_PARAM_PORT:
      ret = _set_port(param, value);
      break;
    case MQTT_PARAM_SUBSCRIBE:
      ret = _set_subscribe(param, value);
      break;
    case MQTT_PARAM_SUBSCRIBE1:
      ret = _set_subscribe1(param, value);
      break;
    case MQTT_PARAM_PUBLISH:
      ret = _set_publish(param, value);
      break;
    default:
      LOGE(MC_PARAM_TAG, "wrong param type");
      break;
  }
  uni_pthread_mutex_unlock(param->mutex);
  return ret;
}

char* MqttParamGet(MqttParam *param, MqttParamType param_type) {
  char *ret = NULL;
  switch (param_type) {
    case MQTT_PARAM_CLIENT_ID:
      ret = param->client_id;
      break;
    case MQTT_PARAM_USERNAME:
      ret = param->username;
      break;
    case MQTT_PARAM_PASSWORD:
      ret = param->password;
      break;
    case MQTT_PARAM_IP:
      ret = param->ip;
      break;
    case MQTT_PARAM_PORT:
      ret = param->port;
      break;
    case MQTT_PARAM_SUBSCRIBE:
      ret = param->subscribe;
      break;
    case MQTT_PARAM_SUBSCRIBE1:
      ret = param->subscribe1;
      break;
    case MQTT_PARAM_PUBLISH:
      ret = param->publish;
      break;
    default:
      LOGE(MC_PARAM_TAG, "wrong param type");
      break;
  }
  return ret;
}

void MqttParamPrint(MqttParam *param) {
  printf("Mqtt Param:"
         "client_id:%s\n"
         "username:%s\n"
         "password:%s\n"
         "ip:%s\n"
         "port:%s\n"
         "subscribe:%s\n"
         "subscribe1:%s\n"
         "publish:%s\n",
         param->client_id,
         param->username,
         param->password,
         param->ip,
         param->port,
         param->subscribe,
         param->subscribe1,
         param->publish);
}

Result MqttParamInit(MqttParam *param) {
  uni_memset(param, 0, sizeof(MqttParam));
  uni_pthread_mutex_init(&param->mutex);
  return E_OK;
}

void MqttParamFinal(MqttParam *param) {
  uni_pthread_mutex_destroy(param->mutex);
}
