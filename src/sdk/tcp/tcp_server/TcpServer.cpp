#include "TcpServer.hpp"
#include "uni_log.h"
#include <iostream>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#define TCP_SERVER_TAG mName.c_str()

TcpServer :: TcpServer(const char *name, int port)
{
    new (this)TcpServer(name, "0.0.0.0", port);
}

TcpServer :: TcpServer(const char *name, string ip, int port)
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    (void)WSAStartup(wVersionRequested, &wsaData);
#endif
    mName = name;
    mIp = ip;
    mPort = port;
    mEventBase = NULL;
    mListener = NULL;
    mPacker = NULL;
    mThread = NULL;
}

TcpServer :: ~TcpServer()
{
    if (mThread != NULL)
    {
        mThread->join();
        delete mThread;
    }
    if (mListener != NULL)
    {
        evconnlistener_free(mListener);
    }
    if (mEventBase != NULL)
    {
        event_base_loopbreak(mEventBase);
        event_base_free(mEventBase);
    }
}

int TcpServer :: processTcpData(TcpServer *server, char *request, int req_len, char *response, int *resp_len) {
  ITcpDataHandler *handler;
  for (vector<ITcpDataHandler *>::iterator it = server->mHandlers.begin(); it != server->mHandlers.end(); it++) {
    handler = *it;
    if (0 == handler->handle(request, req_len, response, resp_len)) {
      LOGT(server->mName.c_str(), "request is handled by %s", handler->getName().c_str());
      break;
    }
  }
  return 0;
}

void TcpServer :: readCb(struct bufferevent *bev, void *arg)
{
    char read_buf[MAX_TCP_RECV_LEN] = {0};
    int read_len;
    char pack_buf[MAX_TCP_PACK_LEN] = {0};
    int pack_len = 0;
    char resp_buf[MAX_TCP_RESP_LEN] = {0};
    int resp_len = 0;
    TcpServer::CbParam *tcp_cb_param = (TcpServer::CbParam *)arg;
    TcpServer *server = tcp_cb_param->server;
    char *ip = inet_ntoa(tcp_cb_param->client->sin_addr);
    read_len = bufferevent_read(bev, read_buf, sizeof(read_buf));
    if (server->mPacker) {
        if (server->mPacker->pack(read_buf, read_len, pack_buf, &pack_len) == 0) {
            processTcpData(server, pack_buf, pack_len, resp_buf, &resp_len);
        }
    } else {
        //no packer,直接处理原始数据
        processTcpData(server, read_buf, read_len, resp_buf, &resp_len);
    }
    cout << "client " << ip << " say:" << read_buf << endl;
    if (resp_len > 0) {
        bufferevent_write(bev, resp_buf, resp_len);
    }
}

// void writeCb(struct bufferevent *bev, void *arg)
// {
//   cout << "I'm 服务器，成功写数据给客户端，写缓冲回调函数被调用..." << endl;
// }

void TcpServer :: eventCb(struct bufferevent *bev, short events, void *arg)
{
    TcpServer::CbParam *tcp_cb_param = (TcpServer::CbParam *)arg;
    TcpServer *server = tcp_cb_param->server;
    LOGT(server->mName.c_str(), "tcp event %d received", events);
    if (events & BEV_EVENT_EOF)
    {
        LOGT(server->mName.c_str(), "connection closed: %s",
             inet_ntoa(tcp_cb_param->client->sin_addr));
        //client什么时候释放的?注意这里会不会有泄漏
        delete tcp_cb_param;
        server->mConnMgr.del(bev);
        bufferevent_free(bev);
    }
    else if (events & BEV_EVENT_ERROR)
    {
        LOGE(server->mName.c_str(), "some other error !");
        delete tcp_cb_param;
        server->mConnMgr.del(bev);
        bufferevent_free(bev);
    }
}

void TcpServer :: listenerCb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr)
{
    struct sockaddr_in *client = (sockaddr_in *)addr;
    TcpServer *server = (TcpServer *)ptr;
    struct event_base *base = server->mEventBase;
    TcpServer::CbParam *tcp_cb_param = new TcpServer::CbParam(server, client);
    //添加新事件
    struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (bev == NULL)
    {
        LOGE(server->mName.c_str(), "create bufferevent failed");
        return;
    }
    char *ip = inet_ntoa(client->sin_addr);
    uint16_t port = ntohs(client->sin_port);
    server->mConnMgr.add(ip, port, bev);
    LOGT(server->mName.c_str(), "connect new client %s:%d", ip, port);
    //给bufferevent缓冲区设置回调
    //bufferevent_setcb(bev, readCb, writeCb, eventCb, inet_ntoa(client->sin_addr));
    bufferevent_setcb(bev, readCb, NULL, eventCb, tcp_cb_param);
    //启动 bufferevent的 读缓冲区。默认是disable的
    bufferevent_enable(bev, EV_READ);
}

void TcpServer :: listenerTask(void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    event_base_dispatch(base);
}

int TcpServer ::listen()
{
    //init server
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(mPort);
    serv.sin_addr.s_addr = inet_addr(mIp.c_str());
    //create event_base
    mEventBase = event_base_new();
    if (mEventBase == NULL)
    {
        LOGE(TCP_SERVER_TAG, "create mEventBase failed");
        return -1;
    }
    //creat socket, bind, recv connection request
    mListener = evconnlistener_new_bind(mEventBase, listenerCb, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                        36, (struct sockaddr *)&serv, sizeof(serv));
    if (mListener == NULL)
    {
        LOGE(TCP_SERVER_TAG, "create mListener failed");
        return -1;
    }
    //start blocking listening
    //event_base_dispatch(mEventBase);
    mThread = new thread(listenerTask, mEventBase);
    return 0;
}

int TcpServer ::setPacker(IProtocolPacker *packer)
{
    mPacker = packer;
    return 0;
}

int TcpServer ::addDataHandler(ITcpDataHandler *handler)
{
    mHandlers.push_back(handler);
    return 0;
}