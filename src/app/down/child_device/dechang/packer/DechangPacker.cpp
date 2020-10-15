#include "DechangPacker.hpp"
#include "uni_log.h"
#include "string.h"

#define DECHANG_PACKER_TAG getName().c_str()

DechangPacker :: DechangPacker() : ITcpPacker("dechang_tcp_packer") {
    mPackLen = 0;
    mDataBuf = DataBufferCreate(MAX_TCP_PACK_LEN * 5);
    LOGT(DECHANG_PACKER_TAG, "packer is created");
}

DechangPacker :: ~DechangPacker() {
    DataBufferDestroy(mDataBuf);
    LOGT(DECHANG_PACKER_TAG, "packer is destroyed");
}

static void _print_pack(const unsigned char *data, int len) {
    printf("pack is:");
    for (int i = 0; i < len; i++) {
        printf("0x%x ", data[i]);
    }
    printf("\n");
}

int DechangPacker :: unpackIn(const char *raw_data, int raw_data_len) {
    int ret = 0;
    int write_size;
    int free_size = DataBufferGetFreeSize(mDataBuf);
    _print_pack((const unsigned char *)raw_data, raw_data_len);
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
    if (mPackLen < 9) { //header 7 + tail 2
        LOGE(DECHANG_PACKER_TAG, "pack is too short(%d)", mPackLen);
        return -1;
    }
    if (mPack[0] != 0x02) {
        LOGE(DECHANG_PACKER_TAG, "STX is not 0x02 but 0x%x", mPack[0]);
        return -1;
    }
    if (mPack[mPackLen - 1] != 0x03) {
        LOGE(DECHANG_PACKER_TAG, "ETX is not 0x03 but 0x%x", mPack[mPackLen - 1]);
        return -1;
    }
    int data_len = (mPack[5] << 8) + mPack[6];
    if (data_len + 9 != mPackLen) {
        LOGE(DECHANG_PACKER_TAG, "datalen(%d) does not match packlen(%d)",
             data_len, mPackLen);
        return -1;
    }
    unsigned char check_sum = 0;
    for (int i = 0; i < mPackLen - 2; i++) {
        check_sum = check_sum ^ mPack[i];
    }
    if (check_sum != mPack[mPackLen - 2]) {
        LOGE(DECHANG_PACKER_TAG, "checksum 0x%x is not expected 0x%x",
             check_sum, mPack[mPackLen - 2]);
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


Message* DechangPacker :: unpackOut() {
    Message* message = NULL;
    unsigned char c;
    while (DataBufferRead((char *)&c, 1, mDataBuf) == 1)
    {
        if (mState == STATE_IDLE) {
            if (c == 0x02) {
                mPack[0] = c;
                mPackLen = 1;
                mState = STATE_STX_RECVED;
            }
        }
        else if (mState == STATE_STX_RECVED)
        {
            mPack[mPackLen] = c;
            mPackLen++;
            if (c == 0x03) {
                mState = STATE_IDLE;
                if (packCheck() != 0) {
                    mPackLen = 0;
                    LOGE(DECHANG_PACKER_TAG, "pack check failed");
                } else {
                    unsigned char cmd_type = mPack[2];
                    switch (cmd_type) {
                        case 0x56: {
                            message = unpackRecvHb();
                            break;
                        }
                        case 0x54: {
                            message = unpackRecvWarn();
                            break;
                        }
                        default:
                        break;
                    }
                    //memcpy(packed_data, mPack, mPackLen);
                    //*packed_data_len = mPackLen;
                    mPackLen = 0;
                    break;
                }
            } else {
                if (mPackLen >= sizeof(mPack)) { //reset
                    mState = STATE_IDLE;
                    mPackLen = 0;
                }
            }
        }
    }
    return message;
}

ITcpPacker * DechangPacker :: copy() {
    ITcpPacker *packer = new DechangPacker();
    return packer;
}

