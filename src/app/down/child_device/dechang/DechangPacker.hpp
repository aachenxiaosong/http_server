#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_PACKER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_PACKER_HPP_

#include <string>
#include "IProtocolPacker.hpp"



using namespace std;

class DechangPacker : public IProtocolPacker {
private:
    char mPack[MAX_TCP_PACK_LEN];
    int mPackLen;
    enum {
        STATE_IDLE = 0,
        STATE_STX_RECVED,
        STATE_CMD_RECVED,
        STATE_LEN_RECVED,
        STATE_CS_RECVED
    } mState;
public:
    DechangPacker();
    ~DechangPacker();
    //0=pack ok, -1=pack failed
    int pack(const char *raw_data, int raw_data_len, char *packed_data, int* packed_data_len);
    //0=unpack ok, -1=unpack failed
    int unpack(const char *packed_data, int packed_data_len, char *raw_data, int* raw_data_len);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_PACKER_HPP_