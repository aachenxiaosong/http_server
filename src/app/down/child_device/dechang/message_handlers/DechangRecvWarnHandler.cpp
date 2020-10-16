#include "DechangRecvWarnHandler.hpp"
#include "TcpConn.hpp"
#include "uni_log.h"
#include <string.h>

#define DECHANG_RECV_WARN_TAG getName().c_str()

DechangRecvWarnHandler :: DechangRecvWarnHandler() : ITcpMessageHandler("dechang_recv_warn_handler") {}

DechangRecvWarnHandler :: ~DechangRecvWarnHandler() {}

int DechangRecvWarnHandler :: handle(const Message &message) {
    if (message.type() != MSG_DECHANG_RECEIVE_WARN) {
        return -1;
    }
    LOGT(DECHANG_RECV_WARN_TAG, "MSG_DECHANG_RECEIVE_WARN message is handled");
    const DechangMessageRecvWarn& msg = (const DechangMessageRecvWarn &)message;
    LOGT(DECHANG_RECV_WARN_TAG, "warn received: index:%d", msg.index());
    DechangMessageRecvWarnAck ack;
    ack.rand(msg.rand());
    ack.cmd(msg.cmd());
    ack.address(msg.address());
    ack.door(msg.door());
    ack.index(msg.index());
    mConn->send(ack);
    return 0;
}