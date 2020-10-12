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

void TcpServer :: readCb(struct bufferevent *bev, void *arg)
{
    char read_buf[MAX_TCP_RECV_LEN] = {0};
    int read_len;
    TcpServer *server = (TcpServer *)arg;
    STcpConn *conn = server->mConnMgr.get(bev);
    read_len = bufferevent_read(bev, read_buf, sizeof(read_buf));
    server->mHandle.onRecv(conn, (const char *)read_buf, read_len);
}

void TcpServer :: eventCb(struct bufferevent *bev, short events, void *arg)
{
    TcpServer *server = (TcpServer *)arg;
    STcpConn *conn = server->mConnMgr.get(bev);
    LOGT(server->mName.c_str(), "tcp event %d received", events);
    if (events & BEV_EVENT_EOF)
    {
        LOGT(server->mName.c_str(), "connection closed: %s:%d",
             conn->ip.c_str(), conn->port);
        //client什么时候释放的?注意这里会不会有泄漏
        server->mConnMgr.del(bev);
        bufferevent_free(bev);
    }
    else if (events & BEV_EVENT_ERROR)
    {
        LOGT(server->mName.c_str(), "some other error of connection: %s:%d",
             conn->ip.c_str(), conn->port);
        server->mConnMgr.del(bev);
        bufferevent_free(bev);
    }
}

void TcpServer :: listenerCb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr)
{
    struct sockaddr_in *client = (sockaddr_in *)addr;
    TcpServer *server = (TcpServer *)ptr;
    struct event_base *base = server->mEventBase;
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
    bufferevent_setcb(bev, readCb, NULL, eventCb, server);
    //启动 bufferevent的 读缓冲区。默认是disable的
    bufferevent_enable(bev, EV_READ);
}

void TcpServer :: dispathTask(void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    event_base_dispatch(base);
}

int TcpServer :: listen()
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
    mThread = new thread(dispathTask, mEventBase);
    return 0;
}

TcpHandle *TcpServer :: getHandle()
{
    return &mHandle;
}

TcpConnMgr* TcpServer :: getConnMgr()
{
    return &mConnMgr;
}