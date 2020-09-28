#ifndef  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_
#define  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_

#include <string>
#include <vector>
#include "IProtocolPacker.hpp"
#include "ITcpRequestHandler.hpp"
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"

using namespace std;

class TcpServer {
private:
    string mIp;
    int mPort;
    vector <ITcpRequestHandler *> mHandlers;
    IProtocolPacker *mPacker;
    struct event_base *mEventBase;
    struct evconnlistener *mListener;

public:
    TcpServer(int port);
    TcpServer(string ip, int port);
    ~TcpServer();
    //new thread
    int listen();
    int setPacker(IProtocolPacker *packer);
    int addRequestHandler(ITcpRequestHandler *handler);
    string getIp();
    int getPort();
};
#endif  //  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_