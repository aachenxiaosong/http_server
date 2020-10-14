#include "DechangServer.hpp"
#include "uni_log.h"

#define DECHANG_SERVER_TAG mName.c_str()

DechangServer :: DechangServer() {
    mName = "dechang_server";
    mTcpServer = new TcpServer("dechang_tcp_server", DECHANG_TCP_SERVER_PORT);
    mTcpServer->setPacker(&mPacker);
    mTcpServer->addReceiver(&mHbReceiver);
    mTcpConnMgr = mTcpServer->getConnMgr();
}

int DechangServer :: run() {
    if (mTcpServer->listen() != 0) {
        LOGE(DECHANG_SERVER_TAG, "tcp listen failed");
        return -1;
    }
    return 0;
}