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

#include "uni_mc_service.h"

#include "uni_msg_center.h"
#include "uni_json.h"
#include "uni_log.h"
#include "configurable_info.h"

#define MC_SERVICE_TAG "mc_service"

#define MAX_RECVER_NUM 32

typedef struct {
  McHandle cp_mc;
  McRecver recvers[MAX_RECVER_NUM];
  int      recver_num;
} McService;

static McService g_mc_service;
pthread_mutex_t  g_recver_mutex = PTHREAD_MUTEX_INITIALIZER;

static void _mc_recv_routine(char *data, int len) {
  int i;
  McRecver recver;
  printf("chen: recv data %s\n", data);
  if (data == NULL || len <= 0) {
    return;
  }
  pthread_mutex_lock(&g_recver_mutex);
  for (i = 0; i < g_mc_service.recver_num; i++) {
    recver = g_mc_service.recvers[i];
    if (recver(data, len) == 0) {
      /* one msg can only be consumed once */
      break;
    }
  }
  pthread_mutex_unlock(&g_recver_mutex);
}

static void _cp_mc_disc_routine() {
  McConnect(g_mc_service.cp_mc, _mc_recv_routine, _cp_mc_disc_routine);
}

Result McServiceConnect(void) {
  Result rc;
  rc = McConnect(g_mc_service.cp_mc, _mc_recv_routine, _cp_mc_disc_routine);
  if (rc != E_OK) {
    LOGE(MC_SERVICE_TAG, "connect platform mc connect failed");
    return rc;
  }
  return rc;
}

void McServiceDisconnect(void) {
  McDisconnect(g_mc_service.cp_mc);
}

Result McServiceInit(void) {
  /* step 1: init g_mc_service */
  memset(&g_mc_service, 0, sizeof(McService));
  g_mc_service.recver_num = 0;
  /* step 3: create connect platform mc */
  g_mc_service.cp_mc = McCreate("cp_mc", MC_REGISTER_URL_CP);
  return E_OK;
}

void McServiceFinal(void) {
  McDestroy(g_mc_service.cp_mc);
  g_mc_service.recver_num = 0;
}

Result McServiceRegisterRecver(McRecver recver) {
  if (NULL == recver) {
    LOGE(MC_SERVICE_TAG, "recv is null");
    return E_FAILED;
  }
  pthread_mutex_lock(&g_recver_mutex);
  if (g_mc_service.recver_num >= MAX_RECVER_NUM) {
    LOGE(MC_SERVICE_TAG, "recver num reaches max value");
    return E_FAILED;
  }
  g_mc_service.recvers[g_mc_service.recver_num] = recver;
  g_mc_service.recver_num++;
  pthread_mutex_unlock(&g_recver_mutex);
  return E_OK;
}
