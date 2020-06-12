/**************************************************************************
 * Copyright (C) 2017-2018  Unisound
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
 * Description : uni_bt_network.h
 * Author      : wanghaohi.unisound.com
 * Date        : 2019.01.29
 *
 **************************************************************************/
#ifndef UTILS_BT_NETWORK_INC_UNI_BT_NETWORK_H_
#define UTILS_BT_NETWORK_INC_UNI_BT_NETWORK_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#define MAX_SSID_LEN (64 + 1)
#define MAX_PASSWORD_LEN (64 + 1)

typedef struct {
  char ssid[MAX_SSID_LEN];
  char password[MAX_PASSWORD_LEN];
} WiFiInfo;

typedef enum {
  WIFI_CONNECTED,
  WIFI_TIMEOUT,
  WIFI_DISCONNECTED,
  WIFI_CONNECTING
}WiFiState;

/**
 * GetUdid will be called when caller tries to get equipment id
 * @udid: char aray to put udid in
 * @len: length of udid
 */
uni_s32 GetUdid(char udid[], int len);

/**
 * SetWifiState will be called if the state changed when connect to WiFi
 * Now 2 states make sense: connected and timeout.
 */
void SetNetworkState(WiFiState state);

/**
 * GetNetworkInfoCallback will be called when softap state changed
 * @code: 0 if the passport is reliable, -1 otherwise
 * @passport: ssid and password of network; valid only if code == 0
 */
typedef void (*GetNetworkInfoCallback)(uni_s32 code, WiFiInfo *passport);
/**
 * BTGetNetworkInfoStart() used to get network passport by softap
 * @passport: the ssid and password of network of softap outputs
 * @return: 0 if success, -1 otherwise
 */
uni_s32 BTGetNetworkInfoStart(GetNetworkInfoCallback callback);
/**
 * BTGetNetworkInfoStop() used to stop getting network passport by softap
 */
uni_s32 BTGetNetworkInfoStop();

/**
 * BtNetworkInit() used to initialize the Bluetooth Network module,
 * register the callbacks to the hal
 */
uni_s32 BtNetworkInit(void);
#ifdef __cplusplus
}
#endif
#endif
