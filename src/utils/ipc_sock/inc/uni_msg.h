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
 * Description : uni_msg.h
 * Author      : guozhenkun@unisound.com
 * Date        : 2018.12.20
 *
 **************************************************************************/
#ifndef UTILS_IPCSOCK_INC_UNI_MSG_H_
#define UTILS_IPCSOCK_INC_UNI_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "list_head.h"
#define MAGIC_CODE               "IPCSOCK"
#define MAGIC_CODE_LEN           (7)
#define IPC_SOCK_SOCKET           "/tmp/ipc_sock.socket"

typedef enum {
  MSG_NOTIFY_TYPE_DATA,
  MSG_NOTIFY_TYPE_CONNECTED,
  MSG_NOTIFY_TYPE_DISCONNECT,
} NotifyType;

typedef struct PayloadHeader {
  char magic[MAGIC_CODE_LEN];
  int  size;
} PayloadHeader;

typedef struct IpcMsg {
  PayloadHeader header;
  char          *data;
  list_head link;
} IpcMsg;

typedef void  (*MsgCallback) (NotifyType type, IpcMsg *msg);
IpcMsg* IpcMsgCreate(char *buf, int size);
int IpcMsgDestroy(IpcMsg *msg);
#ifdef __cplusplus
}
#endif
#endif
