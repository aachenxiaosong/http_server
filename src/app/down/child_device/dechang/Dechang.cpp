#include "Dechang.hpp"
#include "UniLog.hpp"

#define DECHANG_TAG "dechang"

int Dechang :: init() {
    return mServer.run();
}

int Dechang :: onRecvMessage(const string& conn_tag, const Message &message) {
    TcpConn *conn = mServer.getConnMgr()->getByTag(conn_tag);
    if (conn == NULL) {
        LOGE(DECHANG_TAG, "get conn from tag %s failed", conn_tag.c_str());
        return -1;
    }
    return conn->onRecv(message);
}