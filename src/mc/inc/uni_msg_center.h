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
 * Description : uni_msg_center.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.03.31
 *
 **************************************************************************/
#ifndef SDK_CLOUD_MC_INC_UNI_MSG_CENTER_H_
#define SDK_CLOUD_MC_INC_UNI_MSG_CENTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "MQTTClient.h"

//TODO
#define MC_UDID "12345678"
#define MC_APPKEY "gkmgvhnht2gcb4m35vy6emffztfdxdlirtkhhhin"
#define MC_SECKEY "efecfa5a4f75a915140c526e8201832c"
#define MC_TC_DEVICE_ID "u4-testdeviceid1234"
#define MC_SUBSYSTEM_ID "9"
#define MC_DATA_VERSION "v1"
#define MC_APP_OS_TYPE "1"
#define MC_TOKEN ""
#define MC_EXTRAS "extras+params"

typedef void* McHandle;

typedef void (*McRecvHandler) (char *data, int len);
typedef void (*McDiscHandler) (void);

Result   McSend(McHandle handle, char *data, uni_s32 len);
Result   McConnect(McHandle handle, McRecvHandler recv_handler,
                   McDiscHandler disc_handler);
void     McDisconnect(McHandle handle);
McHandle McCreate(const char *name, const char *url);
void     McDestroy(McHandle handle);

#ifdef __cplusplus
}
#endif

#endif  //  SDK_CLOUD_MC_INC_UNI_MSG_CENTER_H_