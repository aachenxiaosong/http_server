#include "DechangPacker.hpp"
#include "uni_log.h"
#include "string.h"

#define DECHANG_PACKER_TAG getName().c_str()

DechangPacker :: DechangPacker() : ITcpPacker("dechang_tcp_packer") {
    mCurLen = 0;
    mDataBuf = DataBufferCreate(MAX_TCP_PACK_LEN * 5);
    LOGT(DECHANG_PACKER_TAG, "packer is created");
}

DechangPacker :: ~DechangPacker() {
    DataBufferDestroy(mDataBuf);
    LOGT(DECHANG_PACKER_TAG, "packer is destroyed");
}

int DechangPacker :: unpackIn(const char *raw_data, int raw_data_len) {
    int ret = 0;
    int write_size;
    int free_size = DataBufferGetFreeSize(mDataBuf);
    if (free_size < raw_data_len) {
        LOGE(DECHANG_PACKER_TAG, "too much data for one time");
        write_size = free_size;
        ret = -1;
    } else {
        write_size = raw_data_len;
    }
    DataBufferWrite(mDataBuf, raw_data, write_size);
    return ret;
}

int DechangPacker :: packCheck() {
    if (mCurLen < 9) { //header 7 + tail 2
        LOGE(DECHANG_PACKER_TAG, "pack is too short(%d)", mCurLen);
        return -1;
    }
    if (mPack[0] != 0x02) {
        LOGE(DECHANG_PACKER_TAG, "STX is not 0x02 but 0x%x", mPack[0]);
        return -1;
    }
    if (mPack[mCurLen - 1] != 0x03) {
        LOGE(DECHANG_PACKER_TAG, "ETX is not 0x03 but 0x%x", mPack[mCurLen - 1]);
        return -1;
    }
    /*int data_len = (mPack[5] << 8) + mPack[6];
    if (data_len + 9 != mCurLen) {
        LOGE(DECHANG_PACKER_TAG, "datalen(%d) does not match packlen(%d)",
             data_len, mCurLen);
        return -1;
    }*/
    unsigned char check_sum = 0;
    for (int i = 0; i < mCurLen - 2; i++) {
        check_sum = check_sum ^ mPack[i];
    }
    if (check_sum != mPack[mCurLen - 2]) {
        LOGE(DECHANG_PACKER_TAG, "checksum 0x%x is not expected 0x%x",
             check_sum, mPack[mCurLen - 2]);
        return -1;
    }
    return 0;
}

DechangMessageRecvHb* DechangPacker :: unpackRecvHb() {
    DechangMessageRecvHb *message = new DechangMessageRecvHb();
    char serial_num[7] = {0};
    message->rand(mPack[1]);
    message->cmd(mPack[2]);
    message->address(mPack[3]);
    message->door(mPack[4]);
    memcpy(serial_num, &mPack[28], 6);
    message->device_id(serial_num);
    message->status(mPack[19]);
    return message;
}

DechangMessageRecvWarn* DechangPacker :: unpackRecvWarn() {
    DechangMessageRecvWarn *message = new DechangMessageRecvWarn();
    message->rand(mPack[1]);
    message->cmd(mPack[2]);
    message->address(mPack[3]);
    message->door(mPack[4]);
    message->index(mPack[16]);
    return message;
}

DechangMessageRecvSwipe* DechangPacker :: unpackRecvSwipe() {
    DechangMessageRecvSwipe *message = new DechangMessageRecvSwipe();
    unsigned long card_no;
    char time_str[20] = {0};
    message->rand(mPack[1]);
    message->cmd(mPack[2]);
    message->address(mPack[3]);
    message->door(mPack[4]);
    card_no = mPack[7] + (mPack[8] << 8) + (mPack[9] << 16) + (mPack[10] << 24);
    message->card_no(card_no);
    snprintf(time_str, sizeof(time_str), "20%02d-%02d-%02d-%02d:%02d:%02d", mPack[16], mPack[15], mPack[14], mPack[13], mPack[12], mPack[11]);
    message->time(time_str);
    message->type(mPack[17]);
    message->door_addr(18);
    message->index(mPack[20]);
    return message;
}

DechangMessageSendCardAck* DechangPacker :: upPackSendCardAck() {
    DechangMessageSendCardAck *message = new DechangMessageSendCardAck();
    message->rand(mPack[1]);
    message->cmd(mPack[2]);
    message->address(mPack[3]);
    message->door(mPack[4]);
    message->ack(mPack[7]);
    return message;
}



Message* DechangPacker :: unpackOut() {
    Message* message = NULL;
    unsigned char c;
    while (DataBufferRead((char *)&c, 1, mDataBuf) == 1)
    {
        if (mState == STATE_IDLE) {
            if (c == 0x02) {
                mPack[0] = c;
                mCurLen = 1;
                mState = STATE_STX_RECVED;
            }
        }
        else if (mState == STATE_STX_RECVED) 
        {
            mPack[mCurLen] = c;
            mCurLen++;
            if (mCurLen == 7) {//7=>head size
                mPackLen = (mPack[5] << 8) + mPack[6];
                mState = STATE_HEAD_RECVED;
            }
        }
        else if (mState == STATE_HEAD_RECVED)
        {
            mPack[mCurLen] = c;
            mCurLen++;
            if (mCurLen == 7 + 2 + mPackLen) {//7+2=>head+tail size
                mState = STATE_IDLE;
                if (packCheck() != 0) {
                    mCurLen = 0;
                    LOGE(DECHANG_PACKER_TAG, "pack check failed");
                } else {
                    unsigned char cmd_type = mPack[2];
                    switch (cmd_type) {
                        case 0x56: { //心跳
                            message = unpackRecvHb();
                            break;
                        }
                        case 0x54: { //警报记录
                            message = unpackRecvWarn();
                            break;
                        }
                        case 0x53: { //刷卡记录
                            message = unpackRecvSwipe();
                            break;
                        }
                        case 0x62: { //增加卡ACK
                            message = upPackSendCardAck();
                            break;
                        }
                        default: {
                            LOGE(DECHANG_PACKER_TAG, "unsupported message type");
                            break;
                        }
                    }
                    //memcpy(packed_data, mPack, mCurLen);
                    //*packed_data_len = mCurLen;
                    mCurLen = 0;
                    break;
                }
            } else {
                if (mCurLen >= sizeof(mPack)) { //reset
                    mState = STATE_IDLE;
                    mCurLen = 0;
                }
            }
        }
    }
    return message;
}

int DechangPacker :: packRecvHbAck(const DechangMessageRecvHbAck &message, char *out_data, int *out_data_len) {
    unsigned char ack[11];
    ack[0] = 0x02;
    ack[1] = message.rand();
    ack[2] = message.cmd();
    ack[3] = message.address();
    ack[4] = message.door();
    ack[5] = 0x2;
    ack[6] = 0x0;
    ack[7] = message.customer_code_h();
    ack[8] = message.customer_code_l();
    ack[9] = 0x0;
    for (int i = 0; i < 9; i++) {
        ack[9] = ack[9] ^ ack[i];
    }
    ack[10] = 0x03;
    memcpy(out_data, ack, 11);
    *out_data_len = 11;
    return 0;
}

int DechangPacker :: packRecvWarnAck(const DechangMessageRecvWarnAck &message, char *out_data, int *out_data_len) {
    unsigned char ack[10];
    ack[0] = 0x02;
    ack[1] = message.rand();
    ack[2] = message.cmd();
    ack[3] = message.address();
    ack[4] = message.door();
    ack[5] = 0x1;
    ack[6] = 0x0;
    ack[7] = message.index();
    ack[8] = 0x0;
    for (int i = 0; i < 8; i++) {
        ack[8] = ack[8] ^ ack[i];
    }
    ack[9] = 0x03;
    memcpy(out_data, ack, 10);
    *out_data_len = 10;
    return 0;
}

int DechangPacker :: packRecvSwipeAck(const DechangMessageRecvSwipeAck &message, char *out_data, int *out_data_len) {
    unsigned char ack[10];
    ack[0] = 0x02;
    ack[1] = message.rand();
    ack[2] = message.cmd();
    ack[3] = message.address();
    ack[4] = message.door();
    ack[5] = 0x1;
    ack[6] = 0x0;
    ack[7] = message.index();
    ack[8] = 0x0;
    for (int i = 0; i < 8; i++) {
        ack[8] = ack[8] ^ ack[i];
    }
    ack[9] = 0x03;
    memcpy(out_data, ack, 10);
    *out_data_len = 10;
    return 0;
}

static void _password_convert(unsigned char *pwd_hex, const char *pwd)
{
  char pwd_a[] = "ffffffff";
  for (int i = 0; i < strlen(pwd) && i < strlen(pwd_a); i++) {
      pwd_a[i] = pwd[i];
  }
  char hex_str[] = "0x00";
  unsigned int n = 0;
  unsigned char c;
  for (int i = 0; i < 4; i++) {
    hex_str[2] = pwd_a[i * 2];
    hex_str[3] = pwd_a[i * 2 + 1]; 
    sscanf(hex_str, "%x", &n);
    pwd_hex[i] = (unsigned char)n;
  }
}

int DechangPacker :: packSendCard(const DechangMessageSendCard &message, char *out_data, int *out_data_len) {
    unsigned char req[36];
    req[0] = 0x02;
    req[1] = 0;
    req[2] = 0x62;
    req[3] = 0;
    req[4] = 0; //门编号为0会不会有问题?
    req[5] = 27;
    req[6] = 0x0;
    //用户
    req[7] = message.user_id() & 0xff;
    req[8] = (message.user_id() >> 8) & 0xff;
    //卡号,协议里是高位在前,实际是低位在前
    /*req[9] = (message.card_no() >> 24) & 0xff;
    req[10] = (message.card_no() >> 16) & 0xff;
    req[11] = (message.card_no() >> 8) & 0xff;
    req[12] = (message.card_no()) & 0xff;*/
    req[9] = (message.card_no()) & 0xff;
    req[10] = (message.card_no() >> 8) & 0xff;
    req[11] = (message.card_no() >> 16) & 0xff;
    req[12] = (message.card_no() >> 24) & 0xff;
    //密码
    _password_convert(&req[13], message.password().c_str());
    //开放时间
    req[17] = (message.door_access()) & 0xff;
    req[18] = (message.door_access() >> 8) & 0xff;
    //未用
    req[19] = req[20] = 0x0;
    //有效期,暂时写死,到时候看expire_date的格式,2050.1.1:0:0
    req[21] = 50;
    req[22] = 1;
    req[23] = 1;
    req[24] = 0;
    req[25] = 0;
    //状态
    //req[26] = message.status();
    snprintf((char *)&req[26], 8, "%s", message.user_name().c_str());
    //checksum
    req[34] = 0;
    for (int i = 0; i < 34; i++)
    {
        req[34] = req[34] ^ req[i];
    }
    req[35] = 0x03;
    memcpy(out_data, req, 36);
    *out_data_len = 36;
    return 0;
}

    
int DechangPacker :: pack(const Message &message,
                          char *out_data, int *out_data_len) {
    switch (message.type()) {
        case MSG_DECHANG_RECEIVE_HB_ACK: {
            return packRecvHbAck((DechangMessageRecvHbAck &)message, out_data, out_data_len);
        }
        case MSG_DECHANG_RECEIVE_WARN_ACK: {
            return packRecvWarnAck((DechangMessageRecvWarnAck &)message, out_data, out_data_len);
        }
        case MSG_DECHANG_RECEIVE_SWIPE_ACK: {
            return packRecvSwipeAck((DechangMessageRecvSwipeAck &)message, out_data, out_data_len);
        }
        case MSG_DECHANG_SEND_CARD: {
            return packSendCard((DechangMessageSendCard &)message, out_data, out_data_len);
        }
        default: {
            LOGE(DECHANG_PACKER_TAG, "unsupport message type %d", message.type());
            break;
        }
    }
    return -1;
}


ITcpPacker * DechangPacker :: copy() {
    ITcpPacker *packer = new DechangPacker();
    return packer;
}

