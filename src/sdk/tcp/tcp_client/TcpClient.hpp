#ifndef  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_
#define  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_

#include "TcpConnMgr.hpp"
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include <string>
#include <thread>

#define MAX_TCP_RECV_LEN 1024

using namespace std;

class TcpClient {
private:
    string mName;
    string mServerIp;
    int mServerPort;

    struct event_base *mEventBase;
    thread *mThread;
    
    //receivers和packer会为每个conn拷贝一份
    vector <ITcpReceiver *> mReceivers;
    ITcpPacker *mPacker;

    TcpConnMgr mConnMgr;

public:
    TcpClient(const char *name, string server_ip, int server_port);
    ~TcpClient();
    int connect();
    int setPacker(ITcpPacker *packer);
    int addReceiver(ITcpReceiver *receiver);
    TcpConnMgr *getConnMgr();


private:
    static void readCb(struct bufferevent *bev, void *arg);
    static void eventCb(struct bufferevent *bev, short events, void *arg);
    static void dispathTask(void *arg);
};
#endif  //  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_