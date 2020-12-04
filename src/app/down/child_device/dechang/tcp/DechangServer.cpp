#include "DechangServer.hpp"
#include "UniLog.hpp"

#define DECHANG_SERVER_TAG mName.c_str()

DechangServer :: DechangServer() {
    mName = "dechang_tcp_server";
    mTcpServer = new TcpServer(mName.c_str(), DECHANG_TCP_SERVER_PORT);
    mTcpServer->setPacker(&mPacker);
    mTcpServer->addHandler(&mRecvHbHandler);
    mTcpServer->addHandler(&mRecvWarnHandler);
    mTcpServer->addHandler(&mRecvSwipeHandler);
    mTcpServer->addHandler(&mSendAddCardHandler);
    mTcpServer->addHandler(&mSendDelCardHandler);
    mTcpServer->addHandler(&mSendDelAllCardHandler);
    mTcpConnMgr = mTcpServer->getConnMgr();
}

DechangServer :: ~DechangServer() {
    if (mTcpServer != NULL) {
        delete mTcpServer;
        mTcpServer = NULL;
    }
}

int DechangServer :: run() {
    if (mTcpServer->listen() != 0) {
        LOGE(DECHANG_SERVER_TAG, "tcp listen failed");
        return -1;
    }
    return 0;
}

TcpConnMgr* DechangServer :: getConnMgr() {
    return mTcpConnMgr;
}
