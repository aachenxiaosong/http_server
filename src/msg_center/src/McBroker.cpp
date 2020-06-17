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

#include "uni_mc.h"
#include "uni_json.h"
#include "uni_log.h"
#include "configurable_info.h"
#include <vector>

#define MC_SERVICE_TAG "mc_service"

#define MAX_RECVER_NUM 32

using namespace std;

typedef struct {
  McHandle cp_mc;
  vector<IMcMsgHandler *> handlers;
} McBroker;

static McBroker g_mc_service;

static void _mc_recv_routine(char *data, int len) {
  int i;
  IMcMsgHandler* handler;
  if (data == NULL || len <= 0) {
    return;
  }
  data[len] = '\0';
  printf("chen: recv data %s len %d\n", data, len);
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

Result McBrokerInit(void) {
  /* step 1: init g_mc_service */
  memset(&g_mc_service, 0, sizeof(McBroker));
  /* step 3: create connect platform mc */
  g_mc_service.cp_mc = McCreate("cp_mc", MC_REGISTER_URL_CP);
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