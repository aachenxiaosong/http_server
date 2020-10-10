#include "TcpClient.hpp"
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

#define TCP_CLIENT_TAG mName.c_str()

TcpClient :: TcpClient(const char *name, string ip, int port)
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    (void)WSAStartup(wVersionRequested, &wsaData);
#endif
    mName = name;
    mServerIp = ip;
    mServerPort = port;
    mEventBase = NULL;
    mEvent = NULL;
    mThread = NULL;
}

TcpClient :: ~TcpClient()
{
    if (mThread != NULL)
    {
        mThread->join();
        delete mThread;
    }
    if (mEvent != NULL) {
        event_free(mEvent);
    }
    if (mEventBase != NULL)
    {
        event_base_loopbreak(mEventBase);
        event_base_free(mEventBase);
    }
}

void TcpClient :: readCb(struct bufferevent *bev, void *arg)
{
    char read_buf[MAX_TCP_RECV_LEN] = {0};
    int read_len;
    TcpClient *client = (TcpClient *)arg;
    read_len = bufferevent_read(bev, read_buf, sizeof(read_buf));
    client->mHandle.onRecv(client->mConnMgr.get(bev), (const char *)read_buf, read_len);
}

void TcpClient :: eventCb(struct bufferevent *bev, short events, void *arg)
{
    TcpClient *client = (TcpClient *)arg;
    LOGT(client->mName.c_str(), "tcp event %d received", events);
    if (events & BEV_EVENT_EOF)
    {
        LOGT(client->mName.c_str(), "connection closed: %s",
             client->mServerIp);
        //client什么时候释放的?注意这里会不会有泄漏
        client->mConnMgr.del(bev);
        bufferevent_free(bev);
    }
    else if (events & BEV_EVENT_ERROR)
    {
        LOGE(client->mName.c_str(), "some other error !");
        client->mConnMgr.del(bev);
        bufferevent_free(bev);
    }
}

int TcpClient ::connect()
{
    int ret;
    struct sockaddr_in serv;
    struct bufferevent *bev = NULL;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(mServerPort);
    serv.sin_addr.s_addr = inet_addr(mServerIp.c_str());
    //create event_base
    mEventBase = event_base_new();
    if (mEventBase == NULL)
    {
        LOGE(TCP_CLIENT_TAG, "create mEventBase failed");
        return -1;
    }
    bev = bufferevent_socket_new(mEventBase, -1, BEV_OPT_CLOSE_ON_FREE);
    if (bev == NULL) {
        LOGE(TCP_CLIENT_TAG, "create bufferevent failed");
        return -1;
    }
    if (0 != bufferevent_socket_connect(bev, (struct sockaddr*)&serv, sizeof(serv))) {
        LOGE(TCP_CLIENT_TAG, "connect server failed");
        return -1;
    }
    mConnMgr.add(mServerIp.c_str(), mServerPort, bev);
    //设置回调
    bufferevent_setcb(bev, readCb, NULL, eventCb, this);
    return 0;
}
