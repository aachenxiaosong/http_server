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
#include "uni_resource_prefetch.hpp"
#include "uni_log.h"
#include "uni_timer.h"
#include "HttpAuth.hpp"
#include "HttpHb.hpp"

#define RES_PREFETCH_TAG "resource_prefetch"


#define REFRESH_PERIOD_SEC           (10)
#define REFRESH_PERIOD_MSEC          (REFRESH_PERIOD_SEC * 1000)
#define HB_PERIOD_MSEC                    (60 * 1000)


Result ResPrefetchInit(void) {
  #if (IGAICLOUD_TYPE == 0)
  LOGT(RES_PREFETCH_TAG, "in test server!!!!!!!");
  #elif (IGAICLOUD_TYPE == 1)
  LOGT(RES_PREFETCH_TAG, "in uat server!!!!!!!");
  #else
  LOGT(RES_PREFETCH_TAG, "in product server!!!!!!!");
  #endif

  /* step1: timer for periodic update */
  HttpAuth::init();
  HttpHb::init();
  return E_OK;
}

void ResPrefetchFinal(void) {
}
