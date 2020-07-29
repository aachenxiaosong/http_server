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
 * Description : uni_resource_prefetch.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.04.01
 *
 **************************************************************************/
#include "uni_resource_prefetch.h"
#include "uni_log.h"
#include "uni_timer.h"
#include "uni_auth_http.h"
#include "uni_hb_http.h"

#define RES_PREFETCH_TAG "resource_prefetch"


#define REFRESH_PERIOD_SEC           (10)
#define REFRESH_PERIOD_MSEC          (REFRESH_PERIOD_SEC * 1000)
#define HB_PERIOD_MSEC                    (60 * 1000)

static struct {
  TimerHandle token_timer;
  TimerHandle hb_timer;
} g_res_prefetch = {0};

static int _token_refresh(void * arg) {
  static int lasting_time = 0;
  Token *token = HttpGetToken();
  if (token == NULL) {
    HttpGetTokenCacheUpdate();
    lasting_time = 0;
  } else {
    lasting_time += REFRESH_PERIOD_SEC;
    if (lasting_time > (token->valid_time >> 1)) {
      HttpGetTokenCacheUpdate();
      lasting_time = 0;
    }
    HttpGetTokenFree(token);
  }
  return 0;
}

static int _hb_report(void *arg) {
  HttpHbReport();
  return 0;
}

Result ResPrefetchInit(void) {
  #if (IGAICLOUD_TYPE == 0)
  LOGT(RES_PREFETCH_TAG, "in test server!!!!!!!");
  #elif (IGAICLOUD_TYPE == 1)
  LOGT(RES_PREFETCH_TAG, "in uat server!!!!!!!");
  #else
  LOGT(RES_PREFETCH_TAG, "in product server!!!!!!!");
  #endif

  /* step1: timer for periodic update */
  g_res_prefetch.token_timer = TimerStart(REFRESH_PERIOD_MSEC,
                                          TIMER_TYPE_PERIODICAL,
                                          _token_refresh,
                                          NULL);
  g_res_prefetch.hb_timer = TimerStart(HB_PERIOD_MSEC,
                                          TIMER_TYPE_PERIODICAL,
                                          _hb_report,
                                          NULL);
  return E_OK;
}

void ResPrefetchFinal(void) {
  TimerStop(g_res_prefetch.token_timer);
  TimerStop(g_res_prefetch.hb_timer);
}
