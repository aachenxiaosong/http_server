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

ITcpPacker * DechangPacker :: copy() {
    ITcpPacker *packer = new DechangPacker();
    return packer;
}

