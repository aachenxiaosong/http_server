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
 * Description : uni_msg_server.h
 * Author      : guozhenkun@unisound.com
 * Date        : 2018.12.20
 *
 **************************************************************************/
#include "ipc_sock_client.h"
#include "uni_iot.h"
#include "uni_ota.h"
#include "uni_msg.h"
#include "list_head.h"
#include "uni_log.h"
#include "ipc_sock_comm.h"

#define IPC_SOCK_CLIENT_TAG      "sock_client"
#define LOOP_INTERVAL_S          (1)

typedef struct IpcSockClient {
  MsgCallback       notify;
  int               sockfd;
  int               is_connected;
  int               is_running;
  uni_sem_t         sem_stopped;
  list_head         msg_list;
  uni_mutex_t       mutex;
} IpcSockClient;

static IpcSockClient *g_sock_client;

static int _socket_set_opt(int sockfd) {
  fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
  return 0;
}

static void _ignore_signal_broken_pipe() {
  struct sigaction new_act;
  memset(&new_act, 0, sizeof(new_act));
  new_act.sa_handler  = SIG_IGN;
  sigemptyset(&new_act.sa_mask);
  sigaction(SIGPIPE, &new_act, NULL);
}

static void _notify_connect() {
  g_sock_client->is_connected = TRUE;
  if (NULL != g_sock_client->notify) {
    g_sock_client->notify(MSG_NOTIFY_TYPE_CONNECTED, NULL);
  }
}

static void _notify_disconnect() {
  if (g_sock_client->is_connected) {
    close(g_sock_client->sockfd);
    g_sock_client->sockfd = -1;
    g_sock_client->is_connected = FALSE;
  }
  if (NULL != g_sock_client->notify) {
    g_sock_client->notify(MSG_NOTIFY_TYPE_DISCONNECT, NULL);
  }
}

static void _notify_data(IpcMsg *msg) {
  if (NULL != g_sock_client->notify) {
    g_sock_client->notify(MSG_NOTIFY_TYPE_DATA, msg);
  }
}

static void _read_server_msg(IpcSockClient *client) {
  IpcMsg *msg = NULL;
  int rc = 0;
  rc = IpcSockReadMsg(client->sockfd, &msg);
  if (IPC_SOCK_CODE_OK == rc) {
    _notify_data(msg);
  } else if (IPC_SOCK_CODE_DISCONN == rc) {
    _notify_disconnect();
  }
  if (NULL != msg) {
    IpcMsgDestroy(msg);
  }
}

static int _connect_server(IpcSockClient *client) {
  int sockfd = -1;
  int len;
  struct sockaddr_un server_addr;
  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, IPC_SOCK_SOCKET);

  LOGT(IPC_SOCK_CLIENT_TAG, "try to connect server");
  if (0 > (sockfd = socket(AF_UNIX, SOCK_STREAM, 0))) {
    LOGE(IPC_SOCK_CLIENT_TAG, "create server socket failed");
    return -1;
  }
  len = offsetof(struct sockaddr_un, sun_path) + strlen(server_addr.sun_path);
  if (0 > connect(sockfd, (struct sockaddr *)&server_addr, len)) {
    close(sockfd);
    LOGE(IPC_SOCK_CLIENT_TAG, "connect server failed: %s", strerror(errno));
    return -1;
  }
  _socket_set_opt(sockfd);
  client->sockfd = sockfd;
  _notify_connect();
  LOGT(IPC_SOCK_CLIENT_TAG, "connect server ok fd = %d", client->sockfd);
  return 0;
}

static inline int _max(int a, int b) {
  return a > b ? a: b;
}

static void _write_msg_to_server(IpcSockClient *client) {
  IpcMsg *msg = NULL;
  int rc = 0;
  uni_pthread_mutex_lock(client->mutex);
  msg = list_get_head_entry(&client->msg_list, IpcMsg, link);
  if (NULL == msg) {
    uni_pthread_mutex_unlock(client->mutex);
    return;
  }
  list_del(&msg->link);
  uni_pthread_mutex_unlock(client->mutex);
  rc = IpcSockWriteMsg(client->sockfd, msg);
  if (IPC_SOCK_CODE_DISCONN == rc) {
    _notify_disconnect();
  }
  IpcMsgDestroy(msg);
}

static void* _msg_process(void *arg) {
  fd_set rfds, wfds;
  int max_fd;
  struct timeval tv;
  IpcSockClient *client = (IpcSockClient *)arg;
  _ignore_signal_broken_pipe();
  while (client->is_running) {
    if (!client->is_connected && 0 != _connect_server(client)) {
        sleep(LOOP_INTERVAL_S * 10);
        continue;
    }
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    max_fd = client->sockfd;
    FD_SET(client->sockfd, &rfds);
    if (!list_empty(&client->msg_list)) {
      FD_SET(client->sockfd, &wfds);
    }
    tv.tv_sec = LOOP_INTERVAL_S;
    tv.tv_usec = 0;
    errno = 0;
    if (0 >= select(max_fd + 1, &rfds, &wfds, NULL, &tv)) {
      continue;
    }
    if (client->is_connected && FD_ISSET(client->sockfd, &rfds)) {
      _read_server_msg(client);
    }
    if (client->is_connected && FD_ISSET(client->sockfd, &wfds)) {
      _write_msg_to_server(client);
    }
  }
  uni_sem_post(client->sem_stopped);
  return NULL;
}

static void _destroy_msg_list(list_head *msg_list) {
  IpcMsg *m, *t;
  list_for_each_entry_safe(m, t, msg_list, IpcMsg, link) {
    list_del(&m->link);
    uni_free(m);
  }
}

static int _create_process_thread() {
  struct thread_param param;
  uni_pthread_t pid;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_DEFAULT_SIZE;
  param.th_priority = OS_PRIORITY_NORMAL;
  uni_strncpy(param.task_name, "sock_client", sizeof(param.task_name) - 1);
  uni_pthread_create(&pid, &param, _msg_process, g_sock_client);
  uni_pthread_detach(pid);
  return 0;
}

int IpcSockClientStart(MsgCallback notify) {
  if (NULL == (g_sock_client = uni_malloc(sizeof(IpcSockClient)))) {
    LOGT(IPC_SOCK_CLIENT_TAG, "sock client start failed");
    return -1;
  }
  memset(g_sock_client, 0, sizeof(IpcSockClient));
  list_init(&g_sock_client->msg_list);
  uni_sem_init(&g_sock_client->sem_stopped, 0);
  uni_pthread_mutex_init(&g_sock_client->mutex);
  g_sock_client->notify = notify;
  g_sock_client->is_running = 1;
  _create_process_thread();
  return 0;

}

int IpcSockClientSend(IpcMsg *msg) {
  if (NULL == msg || !g_sock_client->is_running) {
    return -1;
  }
  uni_pthread_mutex_lock(g_sock_client->mutex);
  list_add_tail(&msg->link, &g_sock_client->msg_list);
  uni_pthread_mutex_unlock(g_sock_client->mutex);
  return 0;
}

int IpcSockClientStop()  {
  g_sock_client->is_running = FALSE;
  uni_sem_wait(g_sock_client->sem_stopped);
  uni_sem_destroy(g_sock_client->sem_stopped);
  uni_pthread_mutex_destroy(g_sock_client->mutex);
  close(g_sock_client->sockfd);
  _destroy_msg_list(&g_sock_client->msg_list);
  uni_free(g_sock_client);
  return 0;
}
