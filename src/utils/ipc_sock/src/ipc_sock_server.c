/**************************************************************************
 * Copyright (C) 2018-2019  Unisound
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
 * Description : ipc_sock_server.h
 * Author      : guozhenkun@unisound.com
 * Date        : 2018.12.20
 *
 **************************************************************************/
#include "ipc_sock_server.h"
#include "ipc_sock_comm.h"
#include "uni_iot.h"
#include "uni_ota.h"
#include "uni_msg.h"
#include "list_head.h"
#include "uni_log.h"

#define IPC_SOCK_SERVER_TAG      "sock_server"
#define LOOP_INTERVAL_S          (1)

typedef struct IpcSockServer {
  MsgCallback      notify;
  int          sockfd;
  int          clientfd;
  int          client_connected;
  int          is_running;
  sem_t            sem_stopped;
  list_head        msg_list;
  pthread_mutex_t  mutex;
} IpcSockServer;

static IpcSockServer *g_sock_server;

static void _ignore_signal_broken_pipe() {
  struct sigaction new_act;
  memset(&new_act, 0, sizeof(new_act));
  new_act.sa_handler  = SIG_IGN;
  sigemptyset(&new_act.sa_mask);
  sigaction(SIGPIPE, &new_act, NULL);
}

static int _start_listen() {
  int sockfd;
  int len;
  struct sockaddr_un server_addr;
  remove(IPC_SOCK_SOCKET);
  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, IPC_SOCK_SOCKET);

  if (0 > (sockfd = socket(AF_UNIX, SOCK_STREAM, 0))) {
    LOGE(IPC_SOCK_SERVER_TAG, "create sock server socket failed:%s", strerror(errno));
    return -1;
  }
  len = offsetof(struct sockaddr_un, sun_path) + strlen(server_addr.sun_path);
  if (0 > bind(sockfd, (struct sockaddr *)&server_addr, len)) {
    LOGE(IPC_SOCK_SERVER_TAG, "sock server socket bind failed:%s", strerror(errno));
    return -1;
  }
  fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
  g_sock_server->sockfd = sockfd;
  if (0 != listen(g_sock_server->sockfd, 5)) {
    LOGE(IPC_SOCK_SERVER_TAG, "server listen failed: %s", strerror(errno));
    return -1;
  }
  LOGT(IPC_SOCK_SERVER_TAG, "server listen ok: fd = %d", g_sock_server->sockfd);
  return 0;
}

static void _notify_disconnect() {
  if (g_sock_server->client_connected) {
    close(g_sock_server->clientfd);
    g_sock_server->clientfd = -1;
    g_sock_server->client_connected = 0;
  }
  if (NULL != g_sock_server->notify) {
    g_sock_server->notify(MSG_NOTIFY_TYPE_DISCONNECT, NULL);
  }
}

static void _notify_connect() {
  if (NULL != g_sock_server->notify) {
    g_sock_server->notify(MSG_NOTIFY_TYPE_CONNECTED, NULL);
  }
  g_sock_server->client_connected = 1;
}

static void _notify_data(IpcMsg *msg) {
  if (NULL != g_sock_server->notify) {
    g_sock_server->notify(MSG_NOTIFY_TYPE_DATA, msg);
  }
}

static void _read_client_msg(IpcSockServer *server) {
  IpcMsg *msg = NULL;
  int rc = 0;
  rc = IpcSockReadMsg(server->clientfd, &msg);
  if (IPC_SOCK_CODE_DISCONN == rc) {
    _notify_disconnect();
  } else if (IPC_SOCK_CODE_OK == rc) {
    _notify_data(msg);
  }
  if (NULL != msg) {
    IpcMsgDestroy(msg);
  }
}

static void _accept_new_client(IpcSockServer *server) {
  int sockfd = accept(server->sockfd, NULL, NULL);
  if (0 > sockfd) {
    LOGE(IPC_SOCK_SERVER_TAG, "accept client error: %s", strerror(errno));
    return;
  }
  _notify_disconnect();
  server->clientfd = sockfd;
  _notify_connect();
  LOGT(IPC_SOCK_SERVER_TAG, "accept new client, id = %d", server->clientfd);
}

static inline int _max(int a, int b) {
  return a > b ? a: b;
}

static void _send_msg_to_client(IpcSockServer *server) {
  IpcMsg *msg = NULL;
  int rc = -1;
  pthread_mutex_lock(&server->mutex);
  msg = list_get_head_entry(&server->msg_list, IpcMsg, link);
  if (NULL == msg) {
    pthread_mutex_unlock(&server->mutex);
    return;
  }
  list_del(&msg->link);
  pthread_mutex_unlock(&server->mutex);
  rc = IpcSockWriteMsg(server->clientfd, msg);
  if (IPC_SOCK_CODE_DISCONN == rc) {
    _notify_disconnect();
  }
  IpcMsgDestroy(msg);
}

static inline int _reset_fds(IpcSockServer *server, fd_set *rfds, fd_set *wfds) {
  FD_ZERO(rfds);
  FD_ZERO(wfds);
  FD_SET(server->sockfd, rfds);
  int max_fd = _max(server->sockfd, server->clientfd);
  if (!server->client_connected) {
    return max_fd;
  }
  FD_SET(server->clientfd, rfds);
  if (!list_empty(&server->msg_list)) {
    FD_SET(server->clientfd, wfds);
  }
  return max_fd;
}

static void* _sock_server_process(void *arg) {
  fd_set rfds, wfds;
  int max_fd;
  struct timeval tv;
  _ignore_signal_broken_pipe();
  IpcSockServer *server = (IpcSockServer *)arg;
  while (server->is_running) {
    max_fd = _reset_fds(server, &rfds, &wfds);
    tv.tv_sec = LOOP_INTERVAL_S;
    tv.tv_usec = 0;
    errno = 0;
    if (0 >= select(max_fd + 1, &rfds, &wfds, NULL, &tv)) {
      continue;
    }
    if (FD_ISSET(server->sockfd, &rfds)) {
      _accept_new_client(server);
    }
    if (server->client_connected && FD_ISSET(server->clientfd, &rfds)) {
      _read_client_msg(server);
    }
    if (server->client_connected && FD_ISSET(server->clientfd, &wfds)) {
      _send_msg_to_client(server);
    }
  }
  sem_post(&server->sem_stopped);
  return NULL;
}

static int _create_process_thread() {
  pthread_t pid;
  pthread_create(&pid, NULL, _sock_server_process, g_sock_server);
  pthread_detach(pid);
  return 0;
}

static void _destroy_msg_list(list_head *msg_list) {
  IpcMsg *m, *t;
  list_for_each_entry_safe(m, t, msg_list, IpcMsg, link) {
    list_del(&m->link);
    uni_free(m);
  }
}

int IpcSockServerStart(MsgCallback notify) {
  if (NULL == (g_sock_server = uni_malloc(sizeof(IpcSockServer)))) {
    return -1;
  }
  memset(g_sock_server, 0, sizeof(IpcSockServer));
  list_init(&g_sock_server->msg_list);
  pthread_mutex_init(&g_sock_server->mutex, NULL);
  g_sock_server->notify = notify;
  if (0 != _start_listen()) {
    LOGE(IPC_SOCK_SERVER_TAG, "start client failed");
    uni_free(g_sock_server);
    return -1;
  }
  g_sock_server->is_running = 1;
  LOGT(IPC_SOCK_SERVER_TAG, "start sock server OK");
  _create_process_thread();
  return 0;
}

int IpcSockServerSend(IpcMsg *msg) {
  if (NULL == msg || !g_sock_server->is_running) {
    return -1;
  }
  pthread_mutex_lock(&g_sock_server->mutex);
  list_add_tail(&msg->link, &g_sock_server->msg_list);
  pthread_mutex_unlock(&g_sock_server->mutex);
  return 0;
}

int IpcSockServerStop()  {
  g_sock_server->is_running = 0;
  sem_wait(&g_sock_server->sem_stopped);
  sem_destroy(&g_sock_server->sem_stopped);
  pthread_mutex_destroy(&g_sock_server->mutex);
  close(g_sock_server->sockfd);
  _notify_disconnect();
  _destroy_msg_list(&g_sock_server->msg_list);
  uni_free(g_sock_server);
  return 0;
}
