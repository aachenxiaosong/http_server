#ifndef  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_
#define  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_

#include "ITcpMessageHandler.hpp"
#include "TcpConnMgr.hpp"
#include "TcpEnum.hpp"
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include <string>
#include <thread>


using namespace std;

class TcpServer {
private:
    string mName;
    string mIp;
    int mPort;

    struct event_base *mEventBase;
    struct evconnlistener *mListener;
    thread *mThread;
    
    vector <ITcpMessageHandler *> mHandlers;
    //packer会为每个conn拷贝一份,因为每个conn要独立缓存中间处理数据
    ITcpPacker *mPacker;
    
    TcpConnMgr mConnMgr;

public:
    TcpServer(const char *name, int port);
    TcpServer(const char *name, string ip, int port);
    ~TcpServer();
    int listen();
    int setPacker(ITcpPacker *packer);
    int addHandler(ITcpMessageHandler *handler);
    TcpConnMgr *getConnMgr();

private:
    static void readCb(struct bufferevent *bev, void *arg);
    static void eventCb(struct bufferevent *bev, short events, void *arg);
    static void listenerCb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr);
    static void dispathTask(void *arg);
};
#endif  //  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_