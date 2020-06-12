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
 * Description : uni_mc_service.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.04.01
 *
 **************************************************************************/
#ifndef SDK_CLOUD_MC_INC_UNI_MC_SERVICE_H_
#define SDK_CLOUD_MC_INC_UNI_MC_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

#if (IGAICLOUD_TYPE == 0)
#define MC_REGISTER_URL_FEATURE "http://test-route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
//#define MC_REGISTER_URL_CP "http://test-route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#define MC_REGISTER_URL_CP "http://test-route.igaicloud.cn:8088/ym-connect-device/api/auth/operateHomeDeviceClientInfo"
#elif (IGAICLOUD_TYPE == 1)
#define MC_REGISTER_URL_FEATURE "http://uat-route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
//#define MC_REGISTER_URL_CP "http://uat-route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#define MC_REGISTER_URL_CP "http://uat-route.igaicloud.cn:8088/ym-connect-device/api/auth/operateHomeDeviceClientInfo"
#else
#define MC_REGISTER_URL_FEATURE "http://route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
//#define MC_REGISTER_URL_CP "http://route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#define MC_REGISTER_URL_CP "http://route.igaicloud.cn:8088/ym-connect-device/api/auth/operateHomeDeviceClientInfo"
#endif
typedef int (*McRecver) (char *data, int len);

Result McServiceConnect(void);
void   McServiceDisconnect(void);
Result McServiceInit(void);
void   McServiceFinal(void);
Result McServiceRegisterRecver(McRecver recver);

#ifdef __cplusplus
}
#endif
#endif  //  SDK_CLOUD_MC_INC_UNI_MC_SERVICE_H_
