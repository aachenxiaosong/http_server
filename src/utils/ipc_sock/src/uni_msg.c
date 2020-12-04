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
#include "uni_msg.h"
#include "uni_iot.h"
#include "uni_log.h"
#define MSG_TAG                  "ipc_msg"

void _set_msg_header(IpcMsg *msg, int size) {
  msg->header.magic[0] = 'I';
  msg->header.magic[1] = 'P';
  msg->header.magic[2] = 'C';
  msg->header.magic[3] = 'S';
  msg->header.magic[4] = 'O';
  msg->header.magic[5] = 'C';
  msg->header.magic[6] = 'K';
  msg->header.size = size;
}

IpcMsg* IpcMsgCreate(char *buf, int size) {
  IpcMsg *msg = NULL;
  if (NULL == (msg = uni_malloc(sizeof(IpcMsg)))) {
    return NULL;
  }
  memset(msg, 0, sizeof(IpcMsg));
  if (NULL != buf && NULL != (msg->data = uni_malloc(size))) {
    memcpy(msg->data, buf, size);
  } else {
    LOGW(MSG_TAG, "msg data lost!!!");
  }
  _set_msg_header(msg, size);
  return msg;
}

int IpcMsgDestroy(IpcMsg *msg)  {
  if (NULL == msg) {
    return -1;
  }
  if (NULL != msg->data) {
    uni_free(msg->data);
  }
  uni_free(msg);
  return 0;
}
