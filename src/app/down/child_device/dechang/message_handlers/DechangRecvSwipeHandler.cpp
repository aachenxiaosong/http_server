#include "DechangRecvSwipeHandler.hpp"
#include "TcpConn.hpp"
#include "uni_log.h"
#include <string.h>

#define DECHANG_RECV_SWIPE_TAG getName().c_str()

DechangRecvSwipeHandler :: DechangRecvSwipeHandler() : ITcpMessageHandler("dechang_recv_hb_handler") {}

DechangRecvSwipeHandler :: ~DechangRecvSwipeHandler() {}

int DechangRecvSwipeHandler :: handle(const Message &message) {
    if (message.type() != MSG_DECHANG_RECEIVE_SWIPE) {
        return -1;
    }
    LOGT(DECHANG_RECV_SWIPE_TAG, "MSG_DECHANG_RECEIVE_SWIPE message is handled");
    const DechangMessageRecvSwipe& msg = (const DechangMessageRecvSwipe &)message;
    LOGT(DECHANG_RECV_SWIPE_TAG, "swip card received: card_no:%lu time:%s type:%d index:%d",
         msg.card_no(), msg.time().c_str(), msg.type(), msg.index());
    DechangMessageRecvSwipeAck ack;
    ack.rand(msg.rand());
    ack.cmd(msg.cmd());
    ack.address(msg.address());
    ack.door(msg.door());
    ack.index(msg.index());
    mConn->send(ack);
    return 0;
}
