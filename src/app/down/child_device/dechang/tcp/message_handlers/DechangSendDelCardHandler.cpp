#include "DechangSendDelCardHandler.hpp"
#include "TcpConn.hpp"
#include "UniLog.hpp"
#include <string.h>
#include <unistd.h>

#define DECHANG_SEND_DEL_CARD_TAG getName().c_str()

DechangSendDelCardHandler :: DechangSendDelCardHandler() : ITcpMessageHandler("dechang_send_del_card_handler") {
    mAckReceived = 0;
}

DechangSendDelCardHandler :: ~DechangSendDelCardHandler() {}

int DechangSendDelCardHandler :: handle(const Message &message) {
    if (message.type() == MSG_DECHANG_SEND_DEL_CARD)
    {
        LOGT(DECHANG_SEND_DEL_CARD_TAG, "MSG_DECHANG_SEND_DEL_CARD message is handled");
        mAckReceived = 0;
        int wait_time = 1000 * 3; //3s超时
        if (0 == mConn->send(message)) {
            while (mAckReceived == 0 && wait_time > 0) {
                usleep(1000 * 10);
                wait_time -= 10;
            }
            if (mAckReceived == 0) {
                LOGE(DECHANG_SEND_DEL_CARD_TAG, "MSG_DECHANG_SEND_DEL_CARD sent timeout(3s)");
            } else {
                LOGE(DECHANG_SEND_DEL_CARD_TAG, "MSG_DECHANG_SEND_DEL_CARD sent OK");
            }
        } else {
            LOGE(DECHANG_SEND_DEL_CARD_TAG, "MSG_DECHANG_SEND_DEL_CARD sent failed");
        }
        return 0;
    } else if (message.type() == MSG_DECHANG_SEND_DEL_CARD_ACK) {
        const DechangMessageSendDelCardAck& msg = dynamic_cast<const DechangMessageSendDelCardAck&>(message);
        LOGT(DECHANG_SEND_DEL_CARD_TAG, "MSG_DECHANG_SEND_DEL_CARD_ACK message is received, status is %s",
             msg.ack() == 0x06 ? "OK" : "FAIL");
        return 0;
    }
    return -1;
}
