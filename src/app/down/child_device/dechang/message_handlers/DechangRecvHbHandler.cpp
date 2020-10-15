#include "DechangRecvHbHandler.hpp"
#include "TcpConn.hpp"
#include "uni_log.h"
#include <string.h>

#define DECHANG_RECV_HB_TAG getName().c_str()

DechangRecvHbHandler :: DechangRecvHbHandler() : ITcpMessageHandler("dechang_recv_hb_handler") {}

DechangRecvHbHandler :: ~DechangRecvHbHandler() {}

void DechangRecvHbHandler :: sendAck(const DechangMessageHbAck &message_ack) {

    unsigned char ack[11];
    ack[0] = 0x02;
    ack[1] = message_ack.rand();
    ack[2] = message_ack.cmd();
    ack[3] = message_ack.address();
    ack[4] = message_ack.door();
    ack[5] = 0x2;
    ack[6] = 0x0;
    ack[7] = message_ack.customer_code_h();
    ack[8] = message_ack.customer_code_l();
    ack[9] = 0x0;
    for (int i = 0; i < 9; i++) {
        ack[9] = ack[9] ^ ack[i];
    }
    ack[10] = 0x03;
    mConn->send((const char *)ack, 11);
}

int DechangRecvHbHandler :: handle(const Message &message) {
    if (message.type() != MSG_DECHANG_RECEIVE_HB) {
        return -1;
    }
    LOGT(DECHANG_RECV_HB_TAG, "MSG_DECHANG_RECEIVE_HB message is handled");
    const DechangMessageRecvHb& msg = (DechangMessageRecvHb &)message;
    if (mConn->getTag().empty()) {
        //only set when first hb received
        mConn->setTag(msg.device_id());
    }
    LOGT(DECHANG_RECV_HB_TAG, "hb received: device %s status %d",
         msg.device_id().c_str(), msg.status());
    DechangMessageHbAck ack;
    ack.rand(msg.rand());
    ack.cmd(msg.cmd());
    ack.address(msg.address());
    ack.door(msg.door());
    ack.customer_code_h(0);
    ack.customer_code_l(0);
    sendAck(ack);
    return 0;
}

/*int DechangRecvHbHandler :: onRecv(const char *data, int len) {
    int ret = -1;
    unsigned char cmd_type = data[2];
    if (cmd_type == 0x56) {
        ret = 0;
        char serial_num[7] = {0};
        unsigned char lock_status = 0;
        memcpy(serial_num, &data[28], 6);
        lock_status = data[19];
        LOGT(DECHANG_RECV_HB_TAG, "hb received: device %s status %d", serial_num, lock_status);
        string tag(serial_num);
        mConn->setTag(tag);
        //TODO 第一次则上报设备上线,这里可能还要有另一个模块专门做这件事
        sendAck(data, len);
        return 0;
    }
    return -1;
}*/

/*ITcpReceiver * DechangRecvHbHandler :: copy() {
    ITcpReceiver *receiver = new DechangRecvHbHandler();
    receiver->setConn(mConn);
    return receiver;
}*/