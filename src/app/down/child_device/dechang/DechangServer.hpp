#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_SERVER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_SERVER_HPP_

#include "TcpServer.hpp"
#include "DechangPacker.hpp"
#include "DechangHbReceiver.hpp"
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
    DechangHbReceiver mHbReceiver;
    string mName;
public:
    DechangServer();
    int run();
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_SERVER_HPP_