#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_PACKER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_PACKER_HPP_

#include <string>
#include "ITcpPacker.hpp"
#include "DechangMessage.hpp"
#include "uni_databuf.h"

/*
 *   结构  代号	      描述            说明
 *   包头  STX	     开始位(1)        0x02
 *         Rand	    随机数(1)        保留
 *	       Command  指令(1)          区别不同的指令
 *	       Address  控制器地址(1)     保留
 *	       Door	    门编号号(1)       1-4
 *	       LengthL  数据长度低位(1)    控制器返回高低位颠倒。
 *	       LengthH  数据长度高位(1)
 *   数据   Data     数据(0..N)       指令包含的数据，有些指令没有该部分
 *   包尾   CS       校验(1)	      见下面的校验算法
 *	       ETX       结束(1)	     0x03
*/

using namespace std;

class DechangPacker : public ITcpPacker {
private:
    DataBufHandle mDataBuf;
    unsigned char mPack[MAX_TCP_PACK_LEN];
    int mPackLen;
    enum {
        STATE_IDLE = 0,
        STATE_STX_RECVED
    } mState;
    int packCheck();
    DechangMessageRecvHb* unpackRecvHb();
    DechangMessageRecvWarn* unpackRecvWarn();
public:
    DechangPacker();
    ~DechangPacker();
    //0=pack ok, -1=pack failed
    int unpackIn(const char *raw_data, int raw_data_len);
    //0=pack has output, -1=pack has no output
    Message* unpackOut();
    ITcpPacker *copy();
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_PACKER_HPP_