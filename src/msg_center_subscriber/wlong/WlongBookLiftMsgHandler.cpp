#include "WlongBookLiftMsgHandler.hpp"

#include "uni_log.h"

#define WLONG_BOOK_MSG_TAG "wlong_book_msg"

WlongBookLiftMsgHandler :: WlongBookLiftMsgHandler() : IMcMsgHandler("wlong_book_msg") {
    LOGT(WLONG_BOOK_MSG_TAG, "WlongBookLiftMsgHandler is created");
}

int WlongBookLiftMsgHandler :: handle(string& msg) {
    LOGT(WLONG_BOOK_MSG_TAG, "msg %s is handled by wlong book lift msg handler", msg.c_str());
    return 0;
}