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
 * Description : uni_udp_bcaster.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.03.09
 *
 **************************************************************************/
#ifndef UTILS_UDP_BCASTER_INC_UNI_UDP_BCASTER_H_
#define UTILS_UDP_BCASTER_INC_UNI_UDP_BCASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"


typedef void* UdpBcHandle;
typedef int (*UdpBcasterRecvCb) (char *data, int size, char *src_ip, void *param);

UdpBcHandle UdpBcasterCreate(int port, UdpBcasterRecvCb recv_cb, void *cb_param);
void UdpBcasterDestroy(UdpBcHandle handle);
int UdpBcasterSend(UdpBcHandle handle, char *data, int size); 

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_UDP_BCASTER_INC_UNI_UDP_BCASTER_H_
