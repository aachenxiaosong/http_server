#include "TcpServer.hpp"
#include "uni_log.h"
#include <iostream>
#include <string.h>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#define TCP_SERVER_TAG "tcp_server"

TcpServer :: TcpServer(int port) {
    TcpServer("0.0.0.0", port);
}

TcpServer ::TcpServer(string ip, int port)
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    (void)WSAStartup(wVersionRequested, &wsaData);
#endif
    mIp = ip;
    mPort = port;
    mListener = NULL;
}

TcpServer :: ~TcpServer() {
    event_base_loopbreak(mEventBase);
    evconnlistener_free(mListener);
    event_base_free(mEventBase);
}


void readCb(struct bufferevent *bev, void *arg)
{
  char buf[1024] = {0};
  char* ip = (char*)arg;
  bufferevent_read(bev, buf, sizeof(buf));
  cout << "client " << ip << " say:" << buf << endl;
  //写数据给客户端
  const char *p = "i am server, i received your msg!" ;
  bufferevent_write(bev, p, strlen(p)+1);
}

void writeCb(struct bufferevent *bev, void *arg)
{
  cout << "I'm 服务器，成功写数据给客户端，写缓冲回调函数被调用..." << endl;
}

void eventCb(struct bufferevent *bev, short events, void *arg)
{
  char* ip = (char*)arg;
  LOGT(TCP_SERVER_TAG, "tcp event %d received", events);
  if (events & BEV_EVENT_EOF)
  {
    LOGT(TCP_SERVER_TAG, "connection closed: %s", ip);
    bufferevent_free(bev);
  }
  else if (events & BEV_EVENT_ERROR)
  {
    LOGE(TCP_SERVER_TAG, "some other error !");
    bufferevent_free(bev);
  }
}

void listenerCb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr)
{
  struct sockaddr_in* client = (sockaddr_in*)addr ;
  LOGT(TCP_SERVER_TAG, "connect new client %s:%d", inet_ntoa(client->sin_addr), ntohs(client->sin_port));
  struct event_base *base = (struct event_base*)ptr;
  //添加新事件
  struct bufferevent *bev;
  bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
  if (bev == NULL)
  {
      LOGE(TCP_SERVER_TAG, "create bufferevent failed");
      return;
  }
  //给bufferevent缓冲区设置回调
  bufferevent_setcb(bev, readCb, writeCb, eventCb, inet_ntoa(client->sin_addr));
  //启动 bufferevent的 读缓冲区。默认是disable 的
  bufferevent_enable(bev, EV_READ);
}

void listenerTask(void *arg) {
    struct event_base *base = (struct event_base *)arg;
    event_base_dispatch(base);
}

int TcpServer :: listen() {
    //init server
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(mPort);
    serv.sin_addr.s_addr = inet_addr(mIp.c_str());
    //create event_base
    mEventBase = event_base_new();
    if (mEventBase == NULL) {
        LOGE(TCP_SERVER_TAG, "create mEventBase failed");
        return -1;
    }
    //creat socket, bind, recv connection request
    mListener = evconnlistener_new_bind(mEventBase, listenerCb, mEventBase, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                        36, (struct sockaddr *)&serv, sizeof(serv));
    if (mListener == NULL) {
        LOGE(TCP_SERVER_TAG, "create mListener failed");
        return -1;
    }
    //start blocking listening
    //event_base_dispatch(mEventBase);
    thread(listenerTask, mEventBase);
    return 0;
}

int TcpServer :: setPacker(IProtocolPacker *packer) {
    return 0;
}

int TcpServer :: addRequestHandler(ITcpRequestHandler *handler) {
    return 0;
}