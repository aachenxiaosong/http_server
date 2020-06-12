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
 * Description : main.c
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_softap.h"
#define WIFI_CONNECTED (1)
#define WIFI_DISCONNECTED (0)

#define MAIN_TAG                      "main"

static int g_wifi_status = -1;
static int _wifi_status_change_callback(int event) {
  LOGT(MAIN_TAG, "wifi status change callback called with %d", event);
  g_wifi_status = event;
  return 0;
}

static void _get_network_info_callback(int code, WiFiInfo *network_info) {
  if (0 == code) {
    struct network_connect passport;
    char security[64] = {0};
    strcpy(security, "WPA-PSK");
    passport.ssid = network_info->ssid;
    passport.pwd = network_info->password;
    passport.security = security;
    LOGT(MAIN_TAG, "connect info: ssid[%s]; pwd[%s]; security[%s]",
         passport.ssid, passport.pwd, passport.security);
    uni_hal_wifievent_register_callback(_wifi_status_change_callback);
    uni_hal_connect_to_network(&passport);
  } else {
    LOGE(MAIN_TAG, "get network info failed");
  }
}

static void _wait_connect(int timeout) {
  int time_wait = timeout;
  while (WIFI_CONNECTED != g_wifi_status && time_wait > 0) {
    uni_sleep(1);
    time_wait--;
  }
}

int main() {
  if (0 != LogInitialize(NULL)) {
    printf("%s%d: log module init failed.", __FUNCTION__, __LINE__);
    return -1;
  }
  LogLevelSet(N_LOG_TRACK);
  if (0 == BTGetNetworkInfoStart(_get_network_info_callback)) {
    _wait_connect(300);
    BTGetNetworkInfoStop();
    if (WIFI_CONNECTED == g_wifi_status) {
      LOGT(MAIN_TAG, "connect network success.");
    } else {
      LOGT(MAIN_TAG, "connect network failed.");
    }
  } else {
    LOGE(MAIN_TAG, "network info failed.");
  }
  return 0;
}
