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
 * Description : uni_bt_network.c
 * Author      : wanghaozhi.unisound.com
 * Date        : 2019.01.29
 *
 **************************************************************************/
#include "uni_bt_network.h"
#include "uni_iot.h"
#include "uni_json.h"
#include "uni_log.h"
#include "uni_sys_hal.h"
#include "uni_ble_cfg.h"
#include "uni_dlopen.h"

#if UNI_BLUETOOTH_NET

#define BT_NETWORK_TAG        "bt_network"
#define LEN_UDID              (64)
#define WLAN                  "wlan0"
#define BLECFG_PATH           "/lib/"
#define SYMBOL_NAME(symbol)   #symbol

typedef struct {
  int (*uni_hal_register_BTmsg_Handler)(BT_CFGNET_CALLBACK callback);
  int (*uni_hal_register_BTstate_Handler)(BT_CFGNET_STATE_CALLBACK callback);
  int (*uni_hal_network_BT_open)(void);
  int (*uni_hal_network_BT_close)(void);
  int (*uni_hal_send_BTmsg)(char *msg, int len);
} BlecfgSymbolTable;

typedef struct {
  DlOpenHandle           blecfg;
  BlecfgSymbolTable      blecfg_symbol_table;
  uni_bool               is_active;
  GetNetworkInfoCallback networkInfo_callback;
  WiFiInfo               wifi_info;
} BtNetwork;

static BtNetwork g_bt_network;
static char g_msg_network_connected[] = \
            "{  \"operate\":\"connectWifi\",  \"status\":\"success\" }";
static char g_msg_network_fail[] = \
            "{  \"operate\":\"connectWifi\",  \"status\":\"fail\" }";
static char g_msg_bt_reply[] = \
            "{  \"operate\":\"connectWifi\",  \"udid\":\"%s\" }";

static uni_s32 _get_mac_address(uni_u8 *address, uni_s32 len) {
  uni_s32 sock, ret;
  struct ifreq ifr;
  uni_memset(&ifr, 0, sizeof(ifr));
  if ((sock = uni_socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    LOGE(BT_NETWORK_TAG, "socket create failed");
    return -1;
  }
  uni_strcpy(ifr.ifr_name, WLAN);
  if ((ret = ioctl(sock, SIOCGIFHWADDR, &ifr, sizeof(ifr))) == 0) {
    uni_memcpy(address, ifr.ifr_hwaddr.sa_data, len);
    ret = 0;
  } else {
    LOGE(BT_NETWORK_TAG, "get mac failed");
    ret = -1;
  }
  close(sock);
  return ret;
}

uni_s32 GetUdid(char udid[], int len) {
  uni_u8 mac[6] = {0};
  if (0 != _get_mac_address(mac, 6)) {
    LOGE(BT_NETWORK_TAG, "get mac address failed");
    return -1;
  }
  uni_snprintf(udid, len, "%02X%02X%02X%02X%02X%02X",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return 0;
}

static void _received_passport(void) {
  char json_resp[256] = {0};
  char udid[16] = {0};
  uni_s32 len = 0;
  BlecfgSymbolTable *symbols = &g_bt_network.blecfg_symbol_table;
  g_bt_network.networkInfo_callback(0, &g_bt_network.wifi_info);
  GetUdid(udid, sizeof(udid));
  uni_snprintf(json_resp, sizeof(json_resp), g_msg_bt_reply, udid);
  len = uni_strlen(json_resp);
  LOGT(BT_NETWORK_TAG, "send udid to app: %s", json_resp);
  if (0 != symbols->uni_hal_send_BTmsg(json_resp, len)) {
    LOGE(BT_NETWORK_TAG, "failed send udid to app");
  }
}

static uni_s32 _parse_passport(char *buffer, uni_s32 len) {
  char *ssid = NULL;
  char *password = NULL;
  buffer[len] = '\0';
  cJSON *data_root = cJSON_Parse(buffer);
  if (0 != JsonReadItemString(data_root, "ssid", &ssid)) {
    LOGE(BT_NETWORK_TAG, "ssid not exist");
    goto L_ERROR;
  }
  uni_strncpy(g_bt_network.wifi_info.ssid, ssid,
              sizeof(g_bt_network.wifi_info.ssid) - 1);
  if (0 != JsonReadItemString(data_root, "password", &password)) {
    LOGE(BT_NETWORK_TAG, "password not exist");
    goto L_ERROR;
  }
  uni_strncpy(g_bt_network.wifi_info.password, password,
              sizeof(g_bt_network.wifi_info.password) - 1);
  LOGT(BT_NETWORK_TAG, "received password: ssid = %s; password = %s",
       g_bt_network.wifi_info.ssid, g_bt_network.wifi_info.password);
  _received_passport();
  uni_free(ssid);
  uni_free(password);
  return 0;
L_ERROR:
  if (ssid) {
    uni_free(ssid);
  }
  if (password) {
    uni_free(password);
  }
  if(data_root) {
    cJSON_Delete(data_root);
  }
  return -1;
}

static int _close_bt(void) {
  int ret;
  BlecfgSymbolTable *symbols = &g_bt_network.blecfg_symbol_table;
  uni_msleep(1000);
  LOGT(BT_NETWORK_TAG, "will try close bluetooth");
  ret = symbols->uni_hal_network_BT_close();
  if (ret != 0) {
    LOGE(BT_NETWORK_TAG, "failed close bluetooth");
  }
  g_bt_network.is_active = FALSE;
  return ret;
}

static int _handle_bt_msg(char *msg, int len) {
  msg[len] = '\0';
  LOGT(BT_NETWORK_TAG, "receive msg[%d]: %s", len, msg);
  _parse_passport(msg, len);
  return 0;
}

static int _handle_bt_state(int state) {
  if (state == STATE_CONNECTED) {
    LOGT(BT_NETWORK_TAG, "bluetooth is connected");
  } else if (state == STATE_OPEN) {
    LOGT(BT_NETWORK_TAG, "bluetooth lost connection");
  }
  return 0;
}

void SetNetworkState(WiFiState state) {
  int len = 0;
  BlecfgSymbolTable *symbols = &g_bt_network.blecfg_symbol_table;
  if (!g_bt_network.is_active) {
    return;
  }
  if (WIFI_CONNECTED == state) {
    LOGT(BT_NETWORK_TAG, "has connected to the network");
    len = sizeof(g_msg_network_connected) - 1;
    if (0 != symbols->uni_hal_send_BTmsg(g_msg_network_connected, len)) {
      LOGE(BT_NETWORK_TAG, "failed send success msg");
    }
    _close_bt();
  } else if (WIFI_TIMEOUT == state) {
    LOGT(BT_NETWORK_TAG, "failed to connect to the network");
    len = sizeof(g_msg_network_fail) - 1;
    if (0 != symbols->uni_hal_send_BTmsg(g_msg_network_fail, len)) {
      LOGE(BT_NETWORK_TAG, "failed send fail msg");
    }
    _close_bt();
  }
}

uni_s32 BTGetNetworkInfoStart(GetNetworkInfoCallback callback) {
  int ret;
  BlecfgSymbolTable *symbols = &g_bt_network.blecfg_symbol_table;
  if (NULL == callback) {
    LOGE(BT_NETWORK_TAG, "callback can not be nil");
    return -1;
  }
  LOGT(BT_NETWORK_TAG, "try to open bluetooth");
  ret = symbols->uni_hal_network_BT_open();
  if (ret != 0) {
    LOGE(BT_NETWORK_TAG, "failed open bluetooth");
  } else {
    char udid[LEN_UDID];
    GetUdid(udid, LEN_UDID);
    LOGT(BT_NETWORK_TAG, "succeed open bluetooth, the udid is %s", udid);
    g_bt_network.is_active = TRUE;
    g_bt_network.networkInfo_callback = callback;
  }
  return ret;
}

uni_s32 BTGetNetworkInfoStop() {
  int ret = _close_bt();
  return ret;
}

static void _close_blecfg_library() {
  DlOpenSharedLibraryClose(g_bt_network.blecfg);
  g_bt_network.blecfg = NULL;
  memset(&g_bt_network.blecfg_symbol_table, 0, sizeof(BlecfgSymbolTable));
}

static int _load_blecfg_library_symbol() {
  BlecfgSymbolTable *symbols = &g_bt_network.blecfg_symbol_table;
  DlOpenHandle blecfg = NULL;
  blecfg = DlOpenLoadSharedLibrary(BLECFG_PATH"libuniblecfg.so");
  if (NULL == blecfg) {
    LOGE(BT_NETWORK_TAG, "load libuniblecfg.so failed");
    return -1;
  }
  g_bt_network.blecfg = blecfg;
  symbols->uni_hal_register_BTmsg_Handler = DlOpenLoadSharedLibrarySymbol(\
      blecfg, SYMBOL_NAME(uni_hal_register_BTmsg_Handler));
  if (NULL == symbols->uni_hal_register_BTmsg_Handler) {
    LOGE(BT_NETWORK_TAG, "load uni_hal_register_BTmsg_Handler symbol failed");
    goto L_ERROR;
  }
  symbols->uni_hal_register_BTstate_Handler = DlOpenLoadSharedLibrarySymbol(\
      blecfg, SYMBOL_NAME(uni_hal_register_BTstate_Handler));
  if (NULL == symbols->uni_hal_register_BTstate_Handler) {
    LOGE(BT_NETWORK_TAG, "load uni_hal_register_BTstate_Handler symbol failed");
    goto L_ERROR;
  }
  symbols->uni_hal_network_BT_open = DlOpenLoadSharedLibrarySymbol(blecfg,
      SYMBOL_NAME(uni_hal_network_BT_open));
  if (NULL == symbols->uni_hal_network_BT_open) {
    LOGE(BT_NETWORK_TAG, "load uni_hal_network_BT_open symbol failed");
    goto L_ERROR;
  }
  symbols->uni_hal_network_BT_close = DlOpenLoadSharedLibrarySymbol(blecfg,
      SYMBOL_NAME(uni_hal_network_BT_close));
  if (NULL == symbols->uni_hal_network_BT_close) {
    LOGE(BT_NETWORK_TAG, "load uni_hal_network_BT_close symbol failed");
    goto L_ERROR;
  }
  symbols->uni_hal_send_BTmsg = DlOpenLoadSharedLibrarySymbol(blecfg,
      SYMBOL_NAME(uni_hal_send_BTmsg));
  if (NULL == symbols->uni_hal_send_BTmsg) {
    LOGE(BT_NETWORK_TAG, "load uni_hal_send_BTmsg symbol failed");
    goto L_ERROR;
  }
  LOGT(BT_NETWORK_TAG, "load uni_hal_send_BTmsg successfully");
  return 0;
L_ERROR:
  _close_blecfg_library();
  return -1;
}

uni_s32 BtNetworkInit(void) {
  int ret = 0;
  BlecfgSymbolTable *symbols = &g_bt_network.blecfg_symbol_table;
  memset(&g_bt_network, 0, sizeof(BtNetwork));
  if (0 != _load_blecfg_library_symbol()) {
    return -1;
  }
  if (0 != symbols->uni_hal_register_BTmsg_Handler(_handle_bt_msg) ||
      0 != symbols->uni_hal_register_BTstate_Handler(_handle_bt_state)) {
    ret = -1;
    LOGE(BT_NETWORK_TAG, "failed register bt callbacks");
  }
  return ret;
}

#else

uni_s32 GetUdid(char udid[], int len) {
  return 0;
}

void SetNetworkState(WiFiState state) {
  return;
}

uni_s32 BTGetNetworkInfoStart(GetNetworkInfoCallback callback) {
  return 0;
}

uni_s32 BTGetNetworkInfoStop() {
  return 0;
}

uni_s32 BtNetworkInit(void) {
  return 0;
}

#endif
