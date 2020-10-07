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
    mPacker = NULL;
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

int TcpClient :: processTcpData(TcpClient *client, char *request, int req_len, char *response, int *resp_len) {
  ITcpDataHandler *handler;
  for (vector<ITcpDataHandler *>::iterator it = client->mHandlers.begin(); it != client->mHandlers.end(); it++) {
    handler = *it;
    if (0 == handler->handle(request, req_len, response, resp_len)) {
      LOGT(client->mName.c_str(), "request is handled by %s", handler->getName().c_str());
      break;
    }
  }
  return 0;
}

void TcpClient :: readCb(struct bufferevent *bev, void *arg)
{
    char read_buf[MAX_TCP_RECV_LEN] = {0};
    int read_len;
    char pack_buf[MAX_TCP_PACK_LEN] = {0};
    int pack_len = 0;
    char resp_buf[MAX_TCP_RESP_LEN] = {0};
    int resp_len = 0;
    TcpClient *client = (TcpClient *)arg;
    read_len = bufferevent_read(bev, read_buf, sizeof(read_buf));
    if (client->mPacker) {
        if (client->mPacker->pack(read_buf, read_len, pack_buf, &pack_len) == 0) {
            processTcpData(client, pack_buf, pack_len, resp_buf, &resp_len);
        }
    } else {
        //no packer,直接处理原始数据
        processTcpData(client, read_buf, read_len, resp_buf, &resp_len);
    }
    cout << "server " << client->mServerIp << " say:" << read_buf << endl;
    if (resp_len > 0) {
        bufferevent_write(bev, resp_buf, resp_len);
    }
}

// void writeCb(struct bufferevent *bev, void *arg)
// {
//   cout << "I'm 服务器，成功写数据给客户端，写缓冲回调函数被调用..." << endl;
// }

void TcpClient :: eventCb(struct bufferevent *bev, short events, void *arg)
{
    TcpClient *client = (TcpClient *)arg;
    LOGT(client->mName.c_str(), "tcp event %d received", events);
    if (events & BEV_EVENT_EOF)
    {
        LOGT(client->mName.c_str(), "connection closed: %s",
             client->mServerIp);
        //client什么时候释放的?注意这里会不会有泄漏
        bufferevent_free(bev);
    }
    else if (events & BEV_EVENT_ERROR)
    {
        LOGE(client->mName.c_str(), "some other error !");
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
    //设置回调
    bufferevent_setcb(bev, readCb, NULL, eventCb, this);
    return 0;
}

int TcpClient ::setPacker(IProtocolPacker *packer)
{
    mPacker = packer;
    return 0;
}

int TcpClient ::addDataHandler(ITcpDataHandler *handler)
{
    mHandlers.push_back(handler);
    return 0;
}