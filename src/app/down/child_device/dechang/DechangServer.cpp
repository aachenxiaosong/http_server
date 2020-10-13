#include "DechangServer.hpp"
#include "uni_log.h"

DechangServer :: DechangServer() {
    mName = "dechang_tcp_server";
    mTcpServer = new TcpServer("dechang_tcp_server", DECHANG_TCP_SERVER_PORT);
    mTcpHandle = mTcpServer->getHandle();
    mTcpConnMgr = mTcpServer->getConnMgr();
    //TODO:注册receiver,考虑下receiver和sender怎么配合,
    //尤其是在server主动发送的时候(是不是可以搞个专门的ack receiver?)
}

int DechangServer :: run() {
    if (mTcpServer->listen() != 0) {
        LOGE(mName.c_str(), "tcp listen failed");
        return -1;
    }
    LOGT(mName.c_str(), "tcp server listening");
    return 0;
}