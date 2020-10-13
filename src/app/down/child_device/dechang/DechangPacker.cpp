#include "DechangPacker.hpp"
#include "uni_log.h"

#define DECHANG_PACKER_TAG getName().c_str()

DechangPacker :: DechangPacker() : IProtocolPacker("dechange_packer") {
    LOGT(DECHANG_PACKER_TAG, "packer is created");
}

DechangPacker :: ~DechangPacker() {
    LOGT(DECHANG_PACKER_TAG, "packer is destroyed");
}

int DechangPacker :: pack(const char *raw_data, int raw_data_len,
                          char *packed_data, int* packed_data_len) {
    int ret = -1;
    switch (mState) {
        case STATE_IDLE:
            break;
        case STATE_STX_RECVED:
        case STATE_CMD_RECVED:
        case STATE_LEN_RECVED:
        case STATE_CS_RECVED:
            break;
        default:
            break;
    }
    return ret;
}
    //0=unpack ok, -1=unpack failed
int DechangPacker :: unpack(const char *packed_data, int packed_data_len,
                            char *raw_data, int* raw_data_len) {

}

