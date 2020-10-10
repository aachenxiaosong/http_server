#ifndef  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_
#define  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_

#include "TcpConnMgr.hpp"
#include "TcpHandle.hpp"
#include <string>
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include <thread>

#define MAX_TCP_RECV_LEN 1024

using namespace std;

class TcpClient {
private:
    string mName;
    string mServerIp;
    int mServerPort;
    TcpConnMgr mConnMgr;
    TcpHandle mHandle;

    struct event_base *mEventBase;
    thread *mThread;

public:
    TcpClient(const char *name, string server_ip, int server_port);
    ~TcpClient();
    int connect();
    TcpHandle *getHandle();
    TcpConnMgr *getConnMgr();


private:
    static void readCb(struct bufferevent *bev, void *arg);
    static void eventCb(struct bufferevent *bev, short events, void *arg);
    static void dispathTask(void *arg);
};
#endif  //  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_