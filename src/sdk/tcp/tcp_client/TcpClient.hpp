#ifndef  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_
#define  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_

#include <string>
#include <vector>
#include "IProtocolPacker.hpp"
#include "ITcpDataHandler.hpp"
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include <thread>

#define MAX_TCP_RECV_LEN 1024
#define MAX_TCP_PACK_LEN 4096
#define MAX_TCP_RESP_LEN 4096

using namespace std;

class TcpClient {
private:
    string mName;
    string mServerIp;
    int mServerPort;
    vector <ITcpDataHandler *> mHandlers;
    IProtocolPacker *mPacker;
    struct event_base *mEventBase;
    struct event *mEvent;
    thread *mThread;

public:
    TcpClient(const char *name, string ip, int port);
    ~TcpClient();
    int connect();
    int setPacker(IProtocolPacker *packer);
    int addDataHandler(ITcpDataHandler *handler);

private:
    static int processTcpData(TcpClient *client, char *request, int req_len, char *response, int *resp_len);
    static void readCb(struct bufferevent *bev, void *arg);
    static void eventCb(struct bufferevent *bev, short events, void *arg);
    
};
#endif  //  SDK_TCP_TCP_CLIENT_TCP_CLIENT_HPP_