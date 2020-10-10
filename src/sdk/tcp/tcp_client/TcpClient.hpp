#ifndef  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_
#define  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_

#include "IProtocolPacker.hpp"
#include "TcpConnMgr.hpp"
#include "TcpHandle.hpp"
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include <thread>
#include <string>
#include <vector>

#define MAX_TCP_RECV_LEN 1024
#define MAX_TCP_PACK_LEN 4096
#define MAX_TCP_RESP_LEN 4096

using namespace std;

class TcpClient {
private:
    string mName;
    string mServerIp;
    int mServerPort;
    TcpConnMgr mConnMgr;
    TcpHandle mHandle;
    struct event_base *mEventBase;
    struct event *mEvent;
    thread *mThread;

public:
    TcpClient(const char *name, string ip, int port);
    ~TcpClient();
    int connect();

private:
    static void readCb(struct bufferevent *bev, void *arg);
    static void eventCb(struct bufferevent *bev, short events, void *arg);
    
};
#endif  //  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_