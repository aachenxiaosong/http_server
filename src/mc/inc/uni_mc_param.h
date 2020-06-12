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
 * Description : uni_mc_param.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.03.31
 *
 **************************************************************************/
#ifndef SDK_CLOUD_MC_SRC_UNI_MC_PARAM_H_
#define SDK_CLOUD_MC_SRC_UNI_MC_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

#define SUBSYSTEM_ID_MAX_LEN 2
#define DATA_VERSION_MAX_LEN 16
#define APPKEY_LEN           40
#define SECRETKEY_LEN        32
#define TC_DEVICE_ID_MAX_LEN 128
#define UDID_MAX_LEN         128
#define TOKEN_MAX_LEN        128
#define APP_OS_TYPE_MAX_LEN  2

typedef enum {
  REGISTER_PARAM_SUBSYSTEM_ID,
  REGISTER_PARAM_DATA_VERSION,
  REGISTER_PARAM_APPKEY,
  REGISTER_PARAM_SECRETKEY,
  REGISTER_PARAM_TC_DEVICE_ID,
  REGISTER_PARAM_UDID,
  REGISTER_PARAM_TOKEN,
  REGISTER_PARAM_APP_OS_TYPE,
  REGISTER_PARAM_EXTRAS
} RegisterParamType;

#define CLIENT_ID_LEN      32
#define USERNAME_LEN       32
#define PASSWORD_LEN       32
#define IP_MAX_LEN         64
#define PORT_MAX_LEN       15
#define SUBSCRIBE_MAX_LEN  64
#define PUBLISH_MAX_LEN    64

typedef enum {
  MQTT_PARAM_CLIENT_ID,
  MQTT_PARAM_USERNAME,
  MQTT_PARAM_PASSWORD,
  MQTT_PARAM_IP,
  MQTT_PARAM_PORT,
  MQTT_PARAM_SUBSCRIBE,
  MQTT_PARAM_PUBLISH
} MqttParamType;

typedef struct {
  char subsystem_id[SUBSYSTEM_ID_MAX_LEN + 1];
  char data_version[DATA_VERSION_MAX_LEN + 1];
  char appkey[APPKEY_LEN + 1];
  char secretkey[SECRETKEY_LEN + 1];
  char tc_device_id[TC_DEVICE_ID_MAX_LEN + 1];
  char udid[UDID_MAX_LEN + 1];
  char token[TOKEN_MAX_LEN + 1];
  char app_os_type[APP_OS_TYPE_MAX_LEN + 1];
  char *extras;
  uni_mutex_t mutex;
} RegisterParam;

typedef struct {
  char client_id[CLIENT_ID_LEN + 1];
  char username[USERNAME_LEN + 1];
  char password[PASSWORD_LEN + 1];
  char ip[IP_MAX_LEN + 1];
  char subscribe[SUBSCRIBE_MAX_LEN + 1];
  char publish[PUBLISH_MAX_LEN + 1];
  char port[PORT_MAX_LEN];
  uni_mutex_t mutex;
} MqttParam;

Result RegisterParamSet(RegisterParam *param, RegisterParamType param_type, const char *value);
char* RegisterParamGet(RegisterParam *param, RegisterParamType param_type);
void RegisterParamPrint(RegisterParam *param);
Result RegisterParamInit(RegisterParam *param);
void RegisterParamFinal(RegisterParam *param);

Result MqttParamSet(MqttParam *param, MqttParamType param_type, const char *value);
char* MqttParamGet(MqttParam *param, MqttParamType param_type);
void MqttParamPrint(MqttParam *param);
Result MqttParamInit(MqttParam *param);
void MqttParamFinal(MqttParam *param);

#ifdef __cplusplus
}
#endif
#endif  //  SDK_CLOUD_MC_SRC_UNI_MC_PARAM_H_
