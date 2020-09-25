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
 * Description : uni_mc_service.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.04.01
 *
 **************************************************************************/

#include "McBroker.hpp"

#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "rili_uart_protocol.h"
#include "uni_mc.h"
#include "uni_json.h"
#include "uni_log.h"
#include "configurable_info.h"
#include "uni_device.h"
#include "uni_uuid.h"
#include <vector>

#define MC_SERVICE_TAG "mc_service"

#define MAX_RECVER_NUM 32

using namespace std;

typedef struct {
  McHandle cp_mc;
  int device_info_synced;
  char device_info_req_id[64];
  uni_pthread_t  device_info_sync_thread;
  vector<IMcMsgHandler *> handlers;
} McBroker;

static McBroker g_mc_service;

static int _check_device_info_synced(char *msg) {
  if (NULL == msg || g_mc_service.device_info_synced == 1) {
    return -1;
  }
  if (strstr(msg, "LIFT_CTRL_STATE_RES") && strstr(msg, g_mc_service.device_info_req_id) ) {
    g_mc_service.device_info_synced = 1;
    LOGT(MC_SERVICE_TAG, "device info sync ack recved");
  }
  return 0;
}

static void _mc_recv_routine(char *data, int len) {
  int i;
  IMcMsgHandler* handler;
  if (data == NULL || len <= 0) {
    return;
  }
  printf("chen: recv data %s len %d\n", data, len);
  _check_device_info_synced(data);
  string sdata = data;
  for (vector<IMcMsgHandler *>::iterator it = g_mc_service.handlers.begin();
        it != g_mc_service.handlers.end(); it++) {
    handler = *it;
    if (handler->handle(sdata) == 0) {
      /* one msg can only be consumed once */
      break;
    }
  }
}

static void _cp_mc_disc_routine() {
  McConnect(g_mc_service.cp_mc, _mc_recv_routine, _cp_mc_disc_routine);
}

Result McBrokerConnect(void) {
  Result rc;
  rc = McConnect(g_mc_service.cp_mc, _mc_recv_routine, _cp_mc_disc_routine);
  if (rc != E_OK) {
    LOGE(MC_SERVICE_TAG, "connect platform mc connect failed");
  }
  return rc;
}

void McBrokerDisconnect(void) {
  McDisconnect(g_mc_service.cp_mc);
}

Result McBrokerSend(const string &data) {
  Result rc;
  rc = McSend(g_mc_service.cp_mc, data.c_str(), data.size());
  if (rc != E_OK) {
    LOGE(MC_SERVICE_TAG, "connect platform mc connect failed");
  }
  return rc;
}

/*
{
    "reqId":"734a6a0bfb564df8a34108e3288024e8",
    "timestamp":"1585194053162",
    "entityCode":"SZ19011304S41600118",
    "typeCode":"F1",
    "msgType":"up",
    "attributesEntities":[
        {
            "attributeCode":"intranetNetAddr",
            "value":"http://177.28.12.32"
        }]
}
 */
static int _publish_device_info(McHandle mc) {
    char sdata[1024] = {0};
    char timestamp[16];
    char uuid[UUID_LEN + 1] = {0};
    CJsonObject jinfo;
    string work_mode = "0";
    int hb_status = 1;
    string hb_msg = "null";
    LOGT(MC_SERVICE_TAG, "3P elevator control system heart beat BEGIN ------------------------------------");
    GetUuid(uuid);
    snprintf(g_mc_service.device_info_req_id, sizeof(g_mc_service.device_info_req_id), "%s", uuid);
    time_t timeval;
    time(&timeval);
    snprintf(timestamp, sizeof(timestamp), "%d000", (unsigned int)timeval);
    if (0 == InitInfo::getInfo(jinfo)) {
      jinfo.Get("workMode", work_mode);
      if (work_mode.compare("1") == 0) {
        WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
        if (0 != wlong_lift_ctrl.aliveTest(hb_msg)) {
          hb_status = 1;
        } else {
          hb_status = 0;
        }
      } else if (work_mode.compare("2") == 0) {
        RiliRequestLiftStatus lift_status_request = {0};
        RiliResponseLiftStatus lift_status_response = {0};
        lift_status_request.building_num = 1;
        lift_status_request.lift_num = 1;
        hb_status = rili_protocol_send(RILI_EVENT_LIFT_STATUS, &lift_status_request, &lift_status_response);
        if (hb_status == 0) {
          hb_msg = "OK";
        } else {
          hb_status = 1;
          hb_msg = "Lost Connect";
        }
      } else {
        hb_status = 1;
        hb_msg = "Unknown Work Mode";
      }
    } else {
      hb_status = 1;
      hb_msg = "In Preparing: init info not recved";
    }
    snprintf(sdata, sizeof(sdata),
                "{"
                    "\"reqId\":\"%s\","
                    "\"timestamp\":\"%s\","
                    "\"entityCode\":\"%s\","
                    "\"typeCode\":\"%s\","
                    "\"msgType\":\"up\","
                    "\"attributesEntities\":["
                        "{"
                              "\"attributeCode\":\"intranetNetAddr\","
                              "\"value\":\"%s\""
                        "},"
                        "{"
                              "\"attributeCode\":\"workMode\","
                              "\"value\":\"%s\""
                        "}"
                    "],"
                    "\"status\":{"
                            "\"health\":%d,"
                            "\"description\":\"%s\""
                     "} "
                 "}",  uuid, timestamp, DeviceGetUdid(), DeviceGetType(), DeviceGetServerUrl(),
                 work_mode.c_str(), hb_status, hb_msg.c_str());
    LOGT(MC_SERVICE_TAG, "publish of device info: %s", sdata);
    if (E_OK != McSend(mc, sdata, strlen(sdata) + 1)) {
        LOGE(MC_SERVICE_TAG, "publish of device info failed");
        LOGT(MC_SERVICE_TAG, "3P elevator control system heart beat END ------------------------------------\n");
        return -1;
    }
    LOGT(MC_SERVICE_TAG, "3P elevator control system heart beat END ------------------------------------\n");
    return 0;
}

static void* _device_info_sync_task(void *param) {
  McHandle mc = (McHandle)param;
  g_mc_service.device_info_synced = 0;
  int retry_time = 0;
  //wait first connecting done
  sleep(5);
  while (1) {
    if (g_mc_service.device_info_synced == 0) {
      LOGT(MC_SERVICE_TAG, "syncing device info, retry time = %d", retry_time);      
      if (0 != _publish_device_info(mc)) {
        //wait for retry
        sleep(1);
      } else {
        //wait for return
        sleep(10);
      }
      retry_time++;
    } else {
      //heart beat
      _publish_device_info(mc);
      sleep(60);
    }
  }
  return NULL;
}
static Result _create_device_info_sync_task(McHandle mc) {
  struct thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_DEFAULT_SIZE;
  param.th_priority = OS_PRIORITY_NORMAL;
  uni_strncpy(param.task_name, "mc_long_conn", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&g_mc_service.device_info_sync_thread, &param,
                                          _device_info_sync_task, mc)) {
    LOGE(MC_SERVICE_TAG, "create mqtt recv task failed");
    return E_FAILED;
  }
  uni_pthread_detach(g_mc_service.device_info_sync_thread);
  return E_OK;
}

Result McBrokerInit(void) {
  /* step 1: init g_mc_service */
  memset(&g_mc_service, 0, sizeof(McBroker));
  /* step 3: create connect platform mc */
  g_mc_service.cp_mc = McCreate("cp_mc", MC_REGISTER_URL_CP);
  _create_device_info_sync_task(g_mc_service.cp_mc);
  return E_OK;
}

void McBrokerFinal(void) {
  McDestroy(g_mc_service.cp_mc);
}

Result McBrokerAddHandler(IMcMsgHandler* handler) {
  if (NULL == handler) {
    LOGE(MC_SERVICE_TAG, "recv is null");
    return E_FAILED;
  }
  g_mc_service.handlers.push_back(handler);
  return E_OK;
}

int McBrokerRemoveHandler(string handler_name) {
  IMcMsgHandler *handler;
  for (vector<IMcMsgHandler *>::iterator it = g_mc_service.handlers.begin();
        it != g_mc_service.handlers.end(); it++) {
    handler = *it;
    if (handler->getName().compare(handler_name) == 0) {
      g_mc_service.handlers.erase(it);
      delete handler;
      return 0;
    }
  }
  return -1;
}
