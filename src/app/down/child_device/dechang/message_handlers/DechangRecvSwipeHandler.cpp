#include "DechangRecvSwipeHandler.hpp"
#include "TcpConn.hpp"
#include "uni_log.h"
#include <string.h>

#define DECHANG_RECV_SWIPE_TAG getName().c_str()

DechangRecvSwipeHandler :: DechangRecvSwipeHandler() : ITcpMessageHandler("dechang_recv_hb_handler") {}

DechangRecvSwipeHandler :: ~DechangRecvSwipeHandler() {}

void DechangRecvSwipeHandler :: sendAck(const DechangMessageSwipeAck &message_ack) {
    unsigned char ack[10];
    ack[0] = 0x02;
    ack[1] = message_ack.rand();
    ack[2] = message_ack.cmd();
    ack[3] = message_ack.address();
    ack[4] = message_ack.door();
    ack[5] = 0x1;
    ack[6] = 0x0;
    ack[7] = message_ack.index();
    ack[8] = 0x0;
    for (int i = 0; i < 8; i++) {
        ack[8] = ack[8] ^ ack[i];
    }
    ack[9] = 0x03;
    mConn->send((const char *)ack, 10);
}

int DechangRecvSwipeHandler :: handle(const Message &message) {
    if (message.type() != MSG_DECHANG_RECEIVE_SWIPE) {
        return -1;
    }
    LOGT(DECHANG_RECV_SWIPE_TAG, "MSG_DECHANG_RECEIVE_SWIPE message is handled");
    const DechangMessageRecvSwipe& msg = (DechangMessageRecvSwipe &)message;
    LOGT(DECHANG_RECV_SWIPE_TAG, "swip card received: card_no:%s time:%s type:%d index:%d",
         msg.card_no().c_str(), msg.time().c_str(), msg.type(), msg.index());
    DechangMessageSwipeAck ack;
    ack.rand(msg.rand());
    ack.cmd(msg.cmd());
    ack.address(msg.address());
    ack.door(msg.door());
    ack.index(msg.index());
    sendAck(ack);
    return 0;
}
