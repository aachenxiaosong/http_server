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
 * Description : uni_msg_center.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.03.31
 *
 **************************************************************************/

#include "uni_mc.h"

#include "uni_cloud_utils.h"
#include "uni_mc_param.h"
#include "uni_event_list.h"
#include "uni_auth_http.h"
#include "uni_http.h"
#include "uni_json.h"
#include "uni_log.h"
#include "uni_device.h"
#include "uni_iot.h"

#define MSG_CENTER_TAG "msg_center"

#define MQTT_SEND_BUF_SIZE 10240
#define MQTT_RECV_BUF_SIZE 10240
#define MQTT_CONNECT_TIMEOUT_MS 5000

enum {
  MC_EVENT_SEND,
  MC_EVENT_CONNECT,
  MC_EVENT_DISCONNECT
};

enum {
  MC_TYPE_IG_CLOUD,
  MC_TYPE_CONNECT_PLATFROM
};

typedef struct {
  int                   type;
  char                  name[32];
  char                  url[256];
  RegisterParam         register_param;
  MqttParam             mqtt_param;
  MQTTClient            mqtt_client;
  Network               net;
  unsigned char         mqtt_send_buf[MQTT_SEND_BUF_SIZE];
  unsigned char         mqtt_recv_buf[MQTT_RECV_BUF_SIZE];
  EventListHandle       event_list;
  uni_bool              connected;
  uni_bool              running;
  uni_pthread_t         recv_thread;
  uni_sem_t             wait_stop;
  McRecvHandler         recv_handler;
  McDiscHandler         disc_handler;
} MsgCenter;


static char* _try_get_udid(MsgCenter *mc) {
  RegisterParam *param = &mc->register_param;
  char *udid = RegisterParamGet(param, REGISTER_PARAM_UDID);
  if (0 != uni_strlen(udid)) {
    return udid;
  }
  RegisterParamSet(param, REGISTER_PARAM_UDID, DeviceGetUdid());
  return RegisterParamGet(param, REGISTER_PARAM_UDID);
}

static char* _format_param_ig_cloud(MsgCenter *mc) {
  RegisterParam *param = &mc->register_param;
  int reg_param_len;
  char *reg_param;
  const char *params[10] = {0};
  const char *appkey = RegisterParamGet(param, REGISTER_PARAM_APPKEY);
  const char *secretkey = RegisterParamGet(param, REGISTER_PARAM_SECRETKEY);
  const char *udid = _try_get_udid(mc);
  const char *subsystem_id = RegisterParamGet(param, REGISTER_PARAM_SUBSYSTEM_ID);
  const char *token = RegisterParamGet(param, REGISTER_PARAM_TOKEN);
  const char *tc_device_id = RegisterParamGet(param, REGISTER_PARAM_TC_DEVICE_ID);
  char timestamp[11];
  const char *data_version = RegisterParamGet(param, REGISTER_PARAM_DATA_VERSION);
  const char *app_os_type = RegisterParamGet(param, REGISTER_PARAM_APP_OS_TYPE);
  const char *extras = RegisterParamGet(param, REGISTER_PARAM_EXTRAS);
  char signature[SIGNATURE_LEN + 1];
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
  BuildSignature(params, 10, signature);
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

static Result _parse_ig_cloud_register_result(MsgCenter *mc, char *result) {
  MqttParam *param = &mc->mqtt_param;
  cJSON *jresult = cJSON_Parse(result);
  char *return_code = NULL;
  char *client_id = NULL;
  char *username = NULL;
  char *password = NULL;
  char *subscribe = NULL;
  char *publish = NULL;
  char *ip = NULL;
  int port = 0;
  char port_str[32] = {0};
  if(jresult == NULL)
    return E_FAILED;
  
  JsonReadItemString(jresult, "returnCode", &return_code);
  if (NULL == return_code) {
    LOGT(MSG_CENTER_TAG, "wrong result of register %s", result);
    cJSON_Delete(jresult);
    return E_FAILED;
  }
  if (uni_strcmp("mc_0000", return_code) != 0 &&
      uni_strcmp("mc_0008", return_code) != 0) {
    LOGT(MSG_CENTER_TAG, "register failed, return code=%s", return_code);
    cJSON_Delete(jresult);
    uni_free(return_code);
    return E_FAILED;
  }
  uni_free(return_code);
  /* set mqtt param */
  if(JsonReadItemString(jresult, "result.clientId", &client_id) != 0) {
    LOGE(MSG_CENTER_TAG, "parse clientId err");
    cJSON_Delete(jresult);
    return E_FAILED;
  }
  
  MqttParamSet(param, MQTT_PARAM_CLIENT_ID, client_id);
  uni_free(client_id);
  if(JsonReadItemString(jresult, "result.connection.username", &username) != 0) {
    LOGE(MSG_CENTER_TAG, "parse username err");
    cJSON_Delete(jresult);
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_USERNAME, username);
  uni_free(username);
  if(JsonReadItemString(jresult, "result.connection.password", &password) != 0) {
    LOGE(MSG_CENTER_TAG, "parse password err");
    cJSON_Delete(jresult);
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_PASSWORD, password);
  uni_free(password);
  if(JsonReadItemString(jresult, "result.connection.ip", &ip) != 0) {
    LOGE(MSG_CENTER_TAG, "parse ip err");
    cJSON_Delete(jresult);
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_IP, ip);
  uni_free(ip);
  JsonReadItemInt(jresult, "result.connection.port", &port);
  snprintf(port_str, sizeof(port_str), "%d", port);
  MqttParamSet(param, MQTT_PARAM_PORT, port_str);
  if(JsonReadItemString(jresult, "result.topics.subscribe", &subscribe) != 0) {
    LOGE(MSG_CENTER_TAG, "parse subscribe err");
    cJSON_Delete(jresult);
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_SUBSCRIBE, subscribe);
  uni_free(subscribe);
  if(JsonReadItemString(jresult, "result.topics.publish", &publish) != 0 ) {
    LOGE(MSG_CENTER_TAG, "parse publish err");
    cJSON_Delete(jresult);
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_PUBLISH, publish);
  uni_free(publish);
  cJSON_Delete(jresult);
  return E_OK;
}

static Result _register_internal_ig_cloud(MsgCenter *mc) {
  Result ret = E_FAILED;
  char *reg_param = _format_param_ig_cloud(mc);
  char *result = NULL;
  LOGT(MSG_CENTER_TAG, "mc name=%s url=%s", mc->name, mc->url);
  if (0 != (HttpPost(mc->url, reg_param, &result))) {
    LOGE(MSG_CENTER_TAG, "register failed for network is not connected");
    uni_free(reg_param);
    return E_FAILED;
  }
  LOGT(MSG_CENTER_TAG, "register param %s", reg_param);
  LOGT(MSG_CENTER_TAG, "register result %s", result ? result : "NULL");
  uni_free(reg_param);
  if (result) {
    ret = _parse_ig_cloud_register_result(mc, result);
    uni_free(result);
  }
  return ret;
}

static Result _parse_connect_platform_register_result(MsgCenter *mc, char *result) {
  MqttParam *param = &mc->mqtt_param;
  cJSON *jresult = cJSON_Parse(result);
  int return_code = -1;
  char *client_id = NULL;
  const char *udid = _try_get_udid(mc);
  char *password = NULL;
  char *ip = NULL;
  char *port = NULL;
  char subscribe[64] = {0};
  char publish[64] = {0};
  if(jresult == NULL) {
    return E_FAILED;
  }
  
  if (0 != JsonReadItemInt(jresult, "retcode", &return_code)) {
    cJSON_Delete(jresult);
    LOGT(MSG_CENTER_TAG, "wrong result of register %s", result);
    return E_FAILED;
  }
  if (return_code != 0) {
    cJSON_Delete(jresult);
    LOGT(MSG_CENTER_TAG, "register failed, return code=%d", return_code);
    return E_FAILED;
  }
  /* set mqtt param */
  if(JsonReadItemString(jresult, "data.clientId", &client_id) != 0) {
    cJSON_Delete(jresult);
    LOGT(MSG_CENTER_TAG, "paser clientId failed");
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_CLIENT_ID, client_id);
  uni_free(client_id);
  MqttParamSet(param, MQTT_PARAM_USERNAME, udid);
  if(JsonReadItemString(jresult, "data.password", &password) != 0)  {
    cJSON_Delete(jresult);
    LOGT(MSG_CENTER_TAG, "paser password failed");
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_PASSWORD, password);
  uni_free(password);
  if(JsonReadItemString(jresult, "data.serviceAddr", &ip) != 0)  {
    cJSON_Delete(jresult);
    LOGT(MSG_CENTER_TAG, "paser addr failed");
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_IP, ip);
  uni_free(ip);
  if(JsonReadItemString(jresult, "data.servicePort", &port) != 0)  {
    cJSON_Delete(jresult);
    LOGT(MSG_CENTER_TAG, "paser port failed");
    return E_FAILED;
  }
  MqttParamSet(param, MQTT_PARAM_PORT, port);
  uni_free(port);
  /* subscribte topic filter: 
   * shimao/ipc/yzs/<udid>/liftControl/command
   * shimao/ipc/yzs/<udid>/liftControl/command/init
   * publish topic filter:
   * shimao/ipc/yzs/<udid>/liftControl/state 
   */
 // snprintf(subscribe, sizeof(subscribe), "shimao/ipc/yzs/%s/liftControl/command/#",
  snprintf(subscribe, sizeof(subscribe), "shimao/ipc/yzs/%s/#",
              DeviceGetUdid());
  LOGT(MSG_CENTER_TAG, "subscribe topic %s", subscribe);
  MqttParamSet(param, MQTT_PARAM_SUBSCRIBE, subscribe);
  snprintf(publish, sizeof(publish), "shimao/ipc/yzs/%s/liftControl/state",
              DeviceGetUdid());
  LOGT(MSG_CENTER_TAG, "publish topic %s", publish);
  MqttParamSet(param, MQTT_PARAM_PUBLISH, publish);
  cJSON_Delete(jresult);
  return E_OK;
}

static int _get_token(char *token) {
  Token *t = HttpGetToken();
  if (t != NULL) {
    strcpy(token, t->token);
    HttpGetTokenFree(t);
    return 0;
  }
  LOGE(MSG_CENTER_TAG, "get token failed");
  return -1;
}

static Result _register_internal_connect_platform(MsgCenter *mc) {
  Result ret = E_FAILED;
  char reg_param[128] = {0};
  char *result = NULL;
  char token[128];
  const char *http_headers[2][2] = {{"Content-Type",
                                     "application/json;charset=UTF-8"},
                                    {NULL, NULL}};
  if (0 != _get_token(token)) {
    return E_FAILED;
  }
  uni_snprintf(reg_param, sizeof(reg_param),
               "{"
                 "\"deviceCode\": \"%s\""
               "}",
               _try_get_udid(mc));
  LOGT(MSG_CENTER_TAG, "mc name=%s url=%s token=%s",
       mc->name, mc->url, token);
  http_headers[1][0] = "token";
  http_headers[1][1] = token;
  if (0 != HttpPostWithHeaders(mc->url, reg_param,
                               (const char * (*)[2])http_headers,
                               sizeof(http_headers) / sizeof(http_headers[0]),
                               &result)) {
    LOGE(MSG_CENTER_TAG, "register failed for network is not connected");
    return E_FAILED;
  }
  LOGT(MSG_CENTER_TAG, "register param %s", reg_param);
  LOGT(MSG_CENTER_TAG, "register result %s", result ? result : "NULL");
  if (result) {
    ret = _parse_connect_platform_register_result(mc, result);
    LOGT(MSG_CENTER_TAG, "register result %p", result);
    uni_free(result);
  }
  return ret;
}

Result _register_internal(MsgCenter *mc) {
  if (mc->type == MC_TYPE_IG_CLOUD) {
    return _register_internal_ig_cloud(mc);
  } else if (mc->type == MC_TYPE_CONNECT_PLATFROM) {
    return _register_internal_connect_platform(mc);
  }
  LOGE(MSG_CENTER_TAG, "wrong msg center type");
  return E_FAILED;
}

static Result _connect_internal(MsgCenter *mc) {
  int ret = 0;
  MqttParam *param = &mc->mqtt_param;
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  LOGT(MSG_CENTER_TAG, "mqtt %s connecting...", mc->name);
  NetworkInit(&mc->net);
  ret = NetworkConnect(&mc->net, MqttParamGet(param, MQTT_PARAM_IP),
                       atoi(MqttParamGet(param, MQTT_PARAM_PORT)));
  if (ret != 0) {
    LOGE(MSG_CENTER_TAG, "net connect failed");
    return E_FAILED;
  }
  MQTTClientInit(&mc->mqtt_client, &mc->net,
                 MQTT_CONNECT_TIMEOUT_MS,
                 mc->mqtt_send_buf, MQTT_SEND_BUF_SIZE,
                 mc->mqtt_recv_buf, MQTT_RECV_BUF_SIZE);
  data.clientID.cstring = MqttParamGet(param, MQTT_PARAM_CLIENT_ID);
  data.username.cstring = MqttParamGet(param, MQTT_PARAM_USERNAME);
  data.password.cstring = MqttParamGet(param, MQTT_PARAM_PASSWORD);
  data.keepAliveInterval = 60;
  data.cleansession = 1;
  ret = MQTTConnect(&mc->mqtt_client, &data, 10, mc->disc_handler);
  if (ret != 0) {
    LOGE(MSG_CENTER_TAG, "mqtt connect failed");
    goto L_ERROR0;
  }
  LOGT(MSG_CENTER_TAG, "mqtt connect succeed, subscribe %s",
       MqttParamGet(param, MQTT_PARAM_SUBSCRIBE));
  ret = MQTTSubscribe(&mc->mqtt_client,
                      MqttParamGet(param, MQTT_PARAM_SUBSCRIBE),
                      QOS0, mc->recv_handler);
  if (ret != 0) {
    LOGE(MSG_CENTER_TAG, "mqtt subscribe failed");
    goto L_ERROR1;
  }
  LOGT(MSG_CENTER_TAG, "mqtt subscribe succeed, will try to refresh data");
  mc->connected = TRUE;
  return E_OK;

L_ERROR1:
  MQTTDisconnect(&mc->mqtt_client);
L_ERROR0:
  NetworkDisconnect(&mc->net);
  return E_FAILED;
}

static Result _disconnect_internal(MsgCenter *mc) {
  if (!mc->connected) {
    return E_FAILED;
  }
  MQTTDisconnect(&mc->mqtt_client);
  NetworkDisconnect(&mc->net);
  mc->connected = FALSE;
  return E_OK;
}

static Result _send_internal(MsgCenter *mc, char *data, uni_s32 len) {
  int ret = 0;
  MQTTMessage msg;
  MqttParam *param = &mc->mqtt_param;
  if (!mc->connected) {
    uni_free(data);
    LOGE(MSG_CENTER_TAG, "mqtt send failed for not connected");
    return E_FAILED;
  }
  uni_memset(&msg, 0, sizeof(msg));
  msg.qos = QOS0;
  msg.payload = data;
  msg.payloadlen = len;
  ret = MQTTPublish(&mc->mqtt_client,
                    MqttParamGet(param, MQTT_PARAM_PUBLISH), &msg);
  uni_free(data);
  if (ret != 0) {
    LOGE(MSG_CENTER_TAG, "mqtt send failed");
    return E_FAILED;
  }
  LOGT(MSG_CENTER_TAG, "mqtt send succedd");
  return E_OK;
}

static void _register_param_init(MsgCenter *mc) {
  RegisterParam *param = &mc->register_param;
  RegisterParamInit(param);
  RegisterParamSet(param, REGISTER_PARAM_SUBSYSTEM_ID, MC_SUBSYSTEM_ID);
  RegisterParamSet(param, REGISTER_PARAM_DATA_VERSION, MC_DATA_VERSION);
  RegisterParamSet(param, REGISTER_PARAM_TC_DEVICE_ID, MC_TC_DEVICE_ID);
  RegisterParamSet(param, REGISTER_PARAM_TOKEN, MC_TOKEN);
  RegisterParamSet(param, REGISTER_PARAM_APP_OS_TYPE, MC_APP_OS_TYPE);
  RegisterParamSet(param, REGISTER_PARAM_EXTRAS, MC_EXTRAS);
   RegisterParamSet(param, REGISTER_PARAM_APPKEY, DeviceGetAppKey());
  RegisterParamSet(param, REGISTER_PARAM_SECRETKEY, DeviceGetSecretKey());
}

static void _mqtt_param_init(MsgCenter *mc) {
  MqttParamInit(&mc->mqtt_param);
}

static void* _long_connect_task(void *arg) {
  MsgCenter *mc = (MsgCenter *)arg;
  prctl(PR_SET_NAME, mc->name, 0, 0, 0);
  while (mc->running) {
    while (mc->connected) {
      if (0 != MQTTYield(&mc->mqtt_client, 1000)) {
        uni_msleep(10);
      }
    }
    while (!mc->connected) {
      if (E_OK != _register_internal(mc)) {
        uni_msleep(2 * 1000);
        continue;
      }
      if (E_OK != _connect_internal(mc)) {
        uni_msleep(2 * 1000);
      }
    }
  }
  if (mc->connected) {
    _disconnect_internal(mc);
  }
  uni_sem_post(mc->wait_stop);
  return NULL;
}

static Result _create_long_connect_task(MsgCenter *mc) {
  struct thread_param param;
  uni_sem_init(&mc->wait_stop, 0);
  mc->running = TRUE;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_DEFAULT_SIZE;
  param.th_priority = OS_PRIORITY_NORMAL;
  uni_strncpy(param.task_name, "mc_long_conn", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&mc->recv_thread, &param,
                              _long_connect_task, mc)) {
    LOGE(MSG_CENTER_TAG, "create mqtt recv task failed");
    return E_FAILED;
  }
  uni_pthread_detach(mc->recv_thread);
  return E_OK;
}

static Result _destroy_long_connect_task(MsgCenter *mc) {
  mc->running = FALSE;
  uni_sem_wait(mc->wait_stop);
  uni_sem_destroy(mc->wait_stop);
  return E_OK;
}

static uni_s32 _cloud_comm_event_handler(Event *event) {
  MsgCenter *mc = (MsgCenter *)event->content.extend_info;
  switch (event->type) {
    case MC_EVENT_SEND:
    {
      FixLengthData *fdata = (FixLengthData *)event->content.info;
      _send_internal(mc, (char *)fdata->data, fdata->length);
      uni_free(fdata);
      break;
    }
    case MC_EVENT_CONNECT:
    {
      if (mc->running) {
        /* disconnection triggers reconnect */
        _disconnect_internal(mc);
      } else {
        _create_long_connect_task(mc);
      }
      break;
    }
    case MC_EVENT_DISCONNECT:
    {
      if (mc->running) {
        _destroy_long_connect_task(mc);
      }
      break;
    }
    default:
      break;
  }
  return 0;
}

Result McSend(McHandle handle, char *data, uni_s32 len) {
  MsgCenter *mc = (MsgCenter *)handle;
  Event *event;
  EventContent event_content;
  FixLengthData *fdata;
  if (NULL == data || len <= 0) {
    LOGE(MSG_CENTER_TAG, "data cannot be empty");
    return E_FAILED;
  }
  fdata = (FixLengthData *)uni_malloc(sizeof(FixLengthData));
  fdata->data = (char *)uni_malloc(len + 1);
  uni_memset(fdata->data, 0, len + 1);
  uni_memcpy(fdata->data, data, len);
  fdata->length = len;
  uni_memset(&event_content, 0, sizeof(EventContent));
  event_content.info = (void *)fdata;
  event_content.extend_info = (void *)mc;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, MC_EVENT_SEND,
                      &event_content, NULL);
  EventListAdd(mc->event_list, event, EVENT_LIST_PRIORITY_MEDIUM);
  EventFree(event);
  return E_OK;
}

Result McConnect(McHandle handle, McRecvHandler recv_handler,
                 McDiscHandler disc_handler) {
  MsgCenter *mc = (MsgCenter *)handle;
  Event *event;
  EventContent event_content;
  mc->recv_handler = recv_handler;
  mc->disc_handler = disc_handler;
  event_content.info = NULL;
  event_content.extend_info = (void *)mc;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, MC_EVENT_CONNECT,
                      &event_content, NULL);
  EventListAdd(mc->event_list, event, EVENT_LIST_PRIORITY_MEDIUM);
  EventFree(event);
  return E_OK;
}

void McDisconnect(McHandle handle) {
  MsgCenter *mc = (MsgCenter *)handle;
  Event *event;
  EventContent event_content;
  event_content.info = NULL;
  event_content.extend_info = (void *)mc;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, MC_EVENT_DISCONNECT,
                      &event_content, NULL);
  EventListAdd(mc->event_list, event, EVENT_LIST_PRIORITY_MEDIUM);
  EventFree(event);
}

McHandle McCreate(const char *name, const char *url) {
  MsgCenter *mc = (MsgCenter *)uni_malloc(sizeof(MsgCenter));
  uni_memset(mc, 0, sizeof(MsgCenter));
  snprintf(mc->name, sizeof(mc->name), "%s", name);
  snprintf(mc->url, sizeof(mc->url), "%s", url);
  if (strstr(mc->url, "operateHomeDeviceClientInfo") != NULL) {
    //api/auth/deviceClientInfo
    mc->type = MC_TYPE_CONNECT_PLATFROM;
  } else {
    //msg-center/rest/v1/client/register
    mc->type = MC_TYPE_IG_CLOUD;
  }
  mc->event_list = EventListCreate(_cloud_comm_event_handler);
  _register_param_init(mc);
  _mqtt_param_init(mc);
  // _try_get_udid();
  return mc;
}

void McDestroy(McHandle handle) {
  MsgCenter *mc = (MsgCenter *)handle;
  RegisterParamFinal(&mc->register_param);
  MqttParamFinal(&mc->mqtt_param);
  EventListDestroy(mc->event_list);
  uni_free(mc);
}
