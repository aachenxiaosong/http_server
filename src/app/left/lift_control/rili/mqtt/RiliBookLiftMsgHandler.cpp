#include "RiliBookLiftMsgHandler.hpp"

#include "uni_log.h"

#define RILI_BOOK_MSG_TAG "rili_book_msg"

RiliBookLiftMsgHandler :: RiliBookLiftMsgHandler() : IMcMsgHandler("rili_book_msg") {
    LOGT(RILI_BOOK_MSG_TAG, "RiliBookLiftMsgHandler is created");
}

int RiliBookLiftMsgHandler :: handle(string& topic, string& msg) {
    LOGT(RILI_BOOK_MSG_TAG, "msg %s is handled by rili book lift msg handler", msg.c_str());
    return 0;
}