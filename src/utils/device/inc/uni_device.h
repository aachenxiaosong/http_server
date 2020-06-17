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
 * Description : uni_device.h
 * Author      : liuwenzheng.unisound.com
 * Date        : 2019.09.27
 *
 **************************************************************************/
#ifndef SDK_DEV_CENTER_INC_UNI_DEVICE_H_
#define SDK_DEV_CENTER_INC_UNI_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

const char* DeviceGetMac(void);
const char* DeviceGetUdid(void);
const char* DeviceGetType(void);
const char* DeviceGetAppKey(void);
const char* DeviceGetSecretKey(void);
const char* DeviceGetServerUrl(void);


#ifdef __cplusplus
}
#endif
#endif /*SDK_DEV_CENTER_INC_UNI_DEVICE_H_*/