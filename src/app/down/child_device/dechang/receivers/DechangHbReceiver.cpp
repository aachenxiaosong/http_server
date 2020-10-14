#include "DechangHbReceiver.hpp"
#include "TcpConn.hpp"
#include "uni_log.h"
#include <string.h>

#define DECHANG_HB_RECVER_TAG mName.c_str()

DechangHbReceiver :: DechangHbReceiver() : ITcpReceiver("dechang_hb_receiver") {}

DechangHbReceiver :: ~DechangHbReceiver() {}

void DechangHbReceiver :: sendAck(const char *data, int len) {
    unsigned char ack[11];
    ack[0] = data[0];
    ack[1] = data[1];
    ack[2] = data[2];
    ack[3] = data[3];
    ack[4] = data[4];
    ack[5] = 0;
    ack[6] = 0x2;
    ack[7] = ack[8] = 0;
    ack[9] = 0;
    for (int i = 0; i < 9; i++) {
        ack[9] = ack[9] ^ ack[i];
    }
    ack[10] = data[len - 1];
    mConn->send((const char *)ack, 11);
}

int DechangHbReceiver :: onRecv(const char *data, int len) {
    int ret = -1;
    unsigned char cmd_type = data[2];
    if (cmd_type == 0x56) {
        ret = 0;
        char serial_num[7] = {0};
        unsigned char lock_status = 0;
        memcpy(serial_num, &data[28], 6);
        lock_status = data[19];
        LOGT(DECHANG_HB_RECVER_TAG, "hb received: device %s status %d", serial_num, lock_status);
        string tag(serial_num);
        mConn->setTag(tag);
        //TODO 第一次则上报设备上线,这里可能还要有另一个模块专门做这件事
        sendAck(data, len);
        return 0;
    }
    return -1;
}

ITcpReceiver * DechangHbReceiver :: copy() {
    ITcpReceiver *receiver = new DechangHbReceiver();
    receiver->setConn(mConn);
    return receiver;
}