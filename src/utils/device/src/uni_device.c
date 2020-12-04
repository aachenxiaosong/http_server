/**************************************************************************
 * Copyright (C) 2019-2019  Unisound
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
 * Description : uni_device.c
 * Author      : liuwenzheng.unisound.com
 * Date        : 2019.09.27
 *
 **************************************************************************/
#include "uni_device.h"
#include "uni_log.h"
#include "configurable_info.h"

#define DEVICE_TAG "device"

static int _get_mac_address(char *address, uni_s32 len) {
#ifdef IS_VMWARE
#if 1
  snprintf(address, len, "%s", "7ccbe2e29774");
#else
  snprintf(address, len, "%s", "7ccbe2e29774_xiaosong1");
#endif
  return 0;
#else
  int sock, ret;
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    LOGE(DEVICE_TAG, "socket create failed");
    return -1;
  }
  strcpy(ifr.ifr_name, NET_NAME);
  if ((ret = ioctl(sock, SIOCGIFHWADDR, &ifr, sizeof(ifr))) == 0) {
    snprintf(address, len, "%02x%02x%02x%02x%02x%02x",
            ifr.ifr_hwaddr.sa_data[0] & 0xff,
            ifr.ifr_hwaddr.sa_data[1] & 0xff,
            ifr.ifr_hwaddr.sa_data[2] & 0xff,
            ifr.ifr_hwaddr.sa_data[3] & 0xff,
            ifr.ifr_hwaddr.sa_data[4] & 0xff,
            ifr.ifr_hwaddr.sa_data[5] & 0xff);
    ret = 0;
  } else {
    LOGE(DEVICE_TAG, "get mac failed");
    ret = -1;
  }
  close(sock);
  LOGT(DEVICE_TAG, "get device mac %s", address);
  return ret;
#endif
}

const char* DeviceGetMac(void) {
  static char mac[64] = {0};
  if (uni_strlen(mac) > 0) {
    return mac;
  }
  if (0 != _get_mac_address(mac, sizeof(mac))) {
    LOGE(DEVICE_TAG, "get mac address failed");
    return NULL;
  }
  return mac;
}

const char* DeviceGetUdid(void) {
  return DeviceGetMac();
  //return "000c29051f71";
}

const char* DeviceGetType(void) {
  return MY_TYPE;
}

const char* DeviceGetAppKey(void) {
  return MY_APPKEY;
}

const char* DeviceGetSecretKey(void) {
  return MY_SECKEY;
}

const char* DeviceGetServerUrl(void) {
  return MY_SERVER_URL;
}
