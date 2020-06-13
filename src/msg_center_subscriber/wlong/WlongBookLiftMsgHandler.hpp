#ifndef MSG_CENTER_SUBSCRIBER_WLONG_WLONG_BOOK_LIFT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_WLONG_WLONG_BOOK_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"

class WlongBookLiftMsgHandler : public IMcMsgHandler {
    public:
    WlongBookLiftMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_WLONG_WLONG_BOOK_LIFT_MSG_HANDLER_H_
