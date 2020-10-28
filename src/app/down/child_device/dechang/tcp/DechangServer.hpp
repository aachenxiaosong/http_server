#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_TCP_DECHANG_SERVER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_TCP_DECHANG_SERVER_HPP_

#include "TcpServer.hpp"
#include "DechangPacker.hpp"
#include "DechangRecvHbHandler.hpp"
#include "DechangRecvWarnHandler.hpp"
#include "DechangRecvSwipeHandler.hpp"
#include "DechangSendAddCardHandler.hpp"
#include "DechangSendDelCardHandler.hpp"
#include "DechangSendDelAllCardHandler.hpp"
#include <string>

//TODO 移到相同的位置
//#define DECHANG_TCP_SERVER_IP "0.0.0.0"
#define DECHANG_TCP_SERVER_PORT 8081

using namespace std;

class DechangServer {
private:
    TcpServer *mTcpServer;
    TcpConnMgr *mTcpConnMgr;
    DechangPacker mPacker;
    DechangRecvHbHandler mRecvHbHandler;
    DechangRecvWarnHandler mRecvWarnHandler;
    DechangRecvSwipeHandler mRecvSwipeHandler;
    DechangSendAddCardHandler mSendAddCardHandler;
    DechangSendDelCardHandler mSendDelCardHandler;
    DechangSendDelAllCardHandler mSendDelAllCardHandler;
    string mName;
public:
    DechangServer();
    ~DechangServer();
    TcpConnMgr *getConnMgr();
    int run();
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_TCP_DECHANG_SERVER_HPP_