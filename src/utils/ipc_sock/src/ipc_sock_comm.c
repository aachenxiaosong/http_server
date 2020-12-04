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
 * Description : ipc_sock_comm.c
 * Author      : guozhenkun@unisound.com
 * Date        : 2018.12.20
 *
 **************************************************************************/
#include "ipc_sock_comm.h"
#include "uni_iot.h"
#include "uni_msg.h"
#include "uni_log.h"

#define SOCK_COMM_TAG   "sock_comm"

static inline int _read_socket(int sockfd, char *buf, int size) {
  int read_size = 0;
  int expected_size = size;
  int rc = -1;
  while (read_size != size && 0 < expected_size) {
    rc = read(sockfd, &buf[read_size], expected_size);
    if (0 >= rc) {
      LOGT(SOCK_COMM_TAG, "read rc = %d, errno = %s", rc, strerror(errno));
      return -1;
    }
    read_size += rc;
    expected_size = size - read_size;
  }
  return 0;
}

static inline int _write_socket(int sockfd, char *buf, int size) {
  int write_size = 0;
  int expected_size = size;
  int rc = -1;

  while (write_size != size && 0 < expected_size) {
    errno = 0;
    rc = write(sockfd, &buf[write_size], expected_size);
    if (0 >= rc && EAGAIN != errno) {
      LOGT(SOCK_COMM_TAG, "write rc = %d, errno = %s", rc, strerror(errno));
      return -1;
    }
    write_size += rc;
    expected_size = size - write_size;
  }
  return 0;
}

int IpcSockReadMsg(int sockfd, IpcMsg **msg) {
  char *data = NULL;
  int header_size = sizeof(PayloadHeader);
  int rc = IPC_SOCK_CODE_OK;
  PayloadHeader header;
  memset(&header, 0, header_size);
  *msg = NULL;

  if (0 != _read_socket(sockfd, (char *)&header, (int)header_size)) {
    return IPC_SOCK_CODE_DISCONN;
  }
  if (0 != memcmp(header.magic, MAGIC_CODE, MAGIC_CODE_LEN)) {
    return IPC_SOCK_CODE_VERIFY;
  }
  header.size = ntohl(header.size);
  if (NULL == (data = uni_malloc(header.size + 1))) {
    return IPC_SOCK_CODE_MALLOC;
  }
  memset(data, 0, header.size + 1);
  if (0 == _read_socket(sockfd, data, header.size)) {
    LOGT(SOCK_COMM_TAG, "receive msg: %s", data);
    *msg = IpcMsgCreate(data, header.size);
  } else {
    rc = IPC_SOCK_CODE_DISCONN;
  }
  uni_free(data);
  return rc;
}

int IpcSockWriteMsg(int sockfd, IpcMsg *msg) {
  int data_size = msg->header.size;
  msg->header.size = htonl(msg->header.size);
  char *t = uni_malloc(data_size + 1);
  if (NULL != t) {
    memset(t, 0, data_size + 1);
    memcpy(t, msg->data, data_size);
    LOGT(SOCK_COMM_TAG, "send msg: %s", t);
    uni_free(t);
  }
  if (0 != _write_socket(sockfd, (char *)&msg->header, (int)sizeof(PayloadHeader))) {
    return IPC_SOCK_CODE_DISCONN;
  }
  if (0 != _write_socket(sockfd, msg->data, data_size)) {
    return IPC_SOCK_CODE_DISCONN;
  }
  return IPC_SOCK_CODE_OK;
}
