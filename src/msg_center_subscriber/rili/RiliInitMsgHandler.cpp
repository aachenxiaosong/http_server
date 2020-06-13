#include "RiliInitMsgHandler.hpp"

#include "uni_log.h"

#define RILI_INIT_MSG_TAG "rili_init_msg"

RiliInitMsgHandler :: RiliInitMsgHandler() : IMcMsgHandler("rili_init_msg") {
    LOGT(RILI_INIT_MSG_TAG, "RiliInitMsgHandler is created");
}

int RiliInitMsgHandler :: handle(string& msg) {
    LOGT(RILI_INIT_MSG_TAG, "msg %s is handled by rili book lift msg handler", msg.c_str());
    return 0;
}