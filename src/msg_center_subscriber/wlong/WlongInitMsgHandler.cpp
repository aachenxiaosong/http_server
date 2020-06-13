#include "WlongInitMsgHandler.hpp"

#include "uni_log.h"

#define WLONG_INIT_MSG_TAG "wlong_init_msg"

WlongInitMsgHandler :: WlongInitMsgHandler() : IMcMsgHandler("wlong_init_msg") {
    LOGT(WLONG_INIT_MSG_TAG, "WlongInitMsgHandler is created");
}

int WlongInitMsgHandler :: handle(string& msg) {
    LOGT(WLONG_INIT_MSG_TAG, "msg %s is handled by wlong book lift msg handler", msg.c_str());
    return 0;
}