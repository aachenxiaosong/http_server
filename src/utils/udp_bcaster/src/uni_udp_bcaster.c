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
 * Description : uni_udp_bcaster.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.03.09
 *
 **************************************************************************/
#include "uni_log.h"
#include "uni_udp_bcaster.h"

//#define UDP_TEST

#define UDP_BCAST_TAG "udp_bcast"

typedef struct {
  int                recv_socket;
#ifdef UDP_TEST
  int                send_socket1;
  struct sockaddr_in send_addr1;
  int                send_socket2;
  struct sockaddr_in send_addr2;
  int                send_socket3;
  struct sockaddr_in send_addr3;
  int                send_socket4;
  struct sockaddr_in send_addr4;
  int                send_socket5;
  struct sockaddr_in send_addr5;
#else
  int                send_socket;
  struct sockaddr_in send_addr;
#endif
  UdpBcasterRecvCb   recv_cb;
  void               *cb_param;
  uni_pthread_t      recv_pid;
  uni_bool           running;
  uni_sem_t          sem_thread_exit_sync;
} UdpBcaster;


static int _receiver_create(int port) {
  int receiver_socket;
  struct sockaddr_in receiver_addr;
  struct timeval tv = {2, 0};
  receiver_socket = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&receiver_addr, 0, sizeof(receiver_addr));
  receiver_addr.sin_family = AF_INET;
  receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  receiver_addr.sin_port = htons(port);
  if(0 != bind(receiver_socket, (struct sockaddr *)&receiver_addr,
               sizeof(receiver_addr))) {
    LOGE(UDP_BCAST_TAG, "bind failed, port is %d err: %s", port, strerror(errno));
    return -1;
  }
  setsockopt(receiver_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv,
             sizeof(struct timeval));
  return receiver_socket;
}

static int _sender_create(struct sockaddr_in *send_addr, int port) {
  int sender_socket;
  int opt_so_broadcast= 1;
  sender_socket = socket(PF_INET,SOCK_DGRAM, 0);
  memset(send_addr, 0, sizeof(struct sockaddr_in));
  send_addr->sin_family = AF_INET;
  send_addr->sin_addr.s_addr = htonl(INADDR_BROADCAST);
  send_addr->sin_port = htons(port);
  setsockopt(sender_socket, SOL_SOCKET, SO_BROADCAST,
             (void *)&opt_so_broadcast, sizeof(opt_so_broadcast));
  return sender_socket;
}

static void* _recv_process(void *prm) {
  UdpBcaster *bcaster = (UdpBcaster *)prm;
  int recv_size;
  char recv_buf[1024];
  struct sockaddr_in from_addr;
  socklen_t addr_len = sizeof(struct sockaddr);
  char *src_ip;
  while (bcaster->running) {
    recv_size = recvfrom(bcaster->recv_socket, recv_buf, sizeof(recv_buf), 0,
                         (struct sockaddr *)&from_addr, &addr_len);
    if(recv_size < 0) {
      if (errno == 11) {
        //recv timeout
        src_ip = NULL;
        recv_buf[0] = '\0';
        bcaster->recv_cb(recv_buf, 0, src_ip, bcaster->cb_param);
      } else {
        LOGE(UDP_BCAST_TAG, "thread exit for recv size < 0 error!!!");
        break;
      }
    } else {
      src_ip = (char *)inet_ntoa(from_addr.sin_addr);
      bcaster->recv_cb(recv_buf, recv_size, src_ip, bcaster->cb_param);
    }
  }
  uni_sem_post(bcaster->sem_thread_exit_sync);
  return NULL;
}

static int _recv_pthread_create(UdpBcaster *bcaster) {
  struct thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_DEFAULT_SIZE;
  param.th_priority = OS_PRIORITY_NORMAL;
  uni_strncpy(param.task_name, "udp_bcaster", sizeof(param.task_name) - 1);
  bcaster->running = TRUE;
  if (0 != uni_pthread_create(&bcaster->recv_pid, &param,
                              _recv_process, bcaster)) {
    LOGE(UDP_BCAST_TAG, "create thread failed");
    return -1;
  }
  uni_pthread_detach(bcaster->recv_pid);
  uni_sem_init(&bcaster->sem_thread_exit_sync, 0);
  return 0;
}

UdpBcHandle UdpBcasterCreate(int port, UdpBcasterRecvCb recv_cb, void *cb_param) {
  UdpBcaster *bcaster = (UdpBcaster *)malloc(sizeof(UdpBcaster));
  if (NULL == bcaster) {
    LOGE(UDP_BCAST_TAG, "memory alloc failed");
    return NULL;
  }
  memset(bcaster, 0, sizeof(UdpBcaster));
  bcaster->recv_socket = _receiver_create(port);
  if (bcaster->recv_socket <= 0) {
    LOGE(UDP_BCAST_TAG, "receiver socket create failed");
    goto L_RECVER_CREAT_FAIL;
  }
#ifdef UDP_TEST
  int port1, port2, port3, port4, port5;
  if (port == 6000) {
    port1 = 6001;
    port2 = 6002;
    port3 = 6003;
    port4 = 6004;
    port5 = 6005;
  } else if (port == 6001) {
    port1 = 6000;
    port2 = 6002;
    port3 = 6003;
    port4 = 6004;
    port5 = 6005;
  } else if (port == 6002) {
    port1 = 6000;
    port2 = 6001;
    port3 = 6003;
    port4 = 6004;
    port5 = 6005;
  } else if (port == 6003) {
    port1 = 6000;
    port2 = 6001;
    port3 = 6002;
    port4 = 6004;
    port5 = 6005;
  } else if (port == 6004) {
    port1 = 6000;
    port2 = 6001;
    port3 = 6002;
    port4 = 6003;
    port5 = 6005;
  } else {
    port1 = 6000;
    port2 = 6001;
    port3 = 6002;
    port4 = 6003;
    port5 = 6004;
  }
  bcaster->send_socket1 = _sender_create(&bcaster->send_addr1, port1);
  bcaster->send_socket2 = _sender_create(&bcaster->send_addr2, port2);
  bcaster->send_socket3 = _sender_create(&bcaster->send_addr3, port3);
  bcaster->send_socket4 = _sender_create(&bcaster->send_addr4, port4);
  bcaster->send_socket5 = _sender_create(&bcaster->send_addr5, port5);
  if (bcaster->send_socket1 <= 0 || bcaster->send_socket2 <= 0 ||
      bcaster->send_socket3 <= 0 || bcaster->send_socket4 <= 0 ||
      bcaster->send_socket5 <= 0) {
    LOGE(UDP_BCAST_TAG, "sender socket create failed");
    goto L_SENDER_CREAT_FAIL;
  }
#else
  bcaster->send_socket = _sender_create(&bcaster->send_addr, port);
  if (bcaster->send_socket <= 0) {
    LOGE(UDP_BCAST_TAG, "sender socket create failed");
    goto L_SENDER_CREAT_FAIL;
  }
#endif
  if (0 != _recv_pthread_create(bcaster)) {
    goto L_THREAD_CREAT_FAIL;
  }
  bcaster->recv_cb = recv_cb;
  bcaster->cb_param = cb_param;
  return bcaster;

L_THREAD_CREAT_FAIL:
#ifdef UDP_TEST
  close(bcaster->send_socket1);
  close(bcaster->send_socket2);
  close(bcaster->send_socket3);
  close(bcaster->send_socket4);
  close(bcaster->send_socket5);
#else
  close(bcaster->send_socket);
#endif
L_SENDER_CREAT_FAIL:
  close(bcaster->recv_socket);
L_RECVER_CREAT_FAIL:
  free(bcaster);
  return NULL;
}

void UdpBcasterDestroy(UdpBcHandle handle) {
  UdpBcaster *bcaster = (UdpBcaster *)handle;
  bcaster->running = FALSE;
  uni_sem_wait(bcaster->sem_thread_exit_sync);
  close(bcaster->recv_socket);
#ifdef UDP_TEST
  close(bcaster->send_socket1);
  close(bcaster->send_socket2);
  close(bcaster->send_socket3);
  close(bcaster->send_socket4);
  close(bcaster->send_socket5);
#else
  close(bcaster->send_socket);
#endif
  uni_sem_destroy(bcaster->sem_thread_exit_sync);
  free(bcaster);
}

int UdpBcasterSend(UdpBcHandle handle, char *data, int size) {
  UdpBcaster *bcaster = (UdpBcaster *)handle;
#ifdef UDP_TEST
  sendto(bcaster->send_socket1, data, size, 0,
         (struct sockaddr *)(&bcaster->send_addr1), sizeof(struct sockaddr_in));
  sendto(bcaster->send_socket2, data, size, 0,
         (struct sockaddr *)(&bcaster->send_addr2), sizeof(struct sockaddr_in));
  sendto(bcaster->send_socket3, data, size, 0,
         (struct sockaddr *)(&bcaster->send_addr3), sizeof(struct sockaddr_in));
  sendto(bcaster->send_socket4, data, size, 0,
         (struct sockaddr *)(&bcaster->send_addr4), sizeof(struct sockaddr_in));
  sendto(bcaster->send_socket5, data, size, 0,
         (struct sockaddr *)(&bcaster->send_addr5), sizeof(struct sockaddr_in));
#else
  sendto(bcaster->send_socket, data, size, 0,
         (struct sockaddr *)(&bcaster->send_addr), sizeof(struct sockaddr_in));
#endif
  return 0;
}

void error_handling(char * message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
