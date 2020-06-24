#ifndef MSG_CENTER_SUBSCRIBER_HANDLERS_RILI_RILI_BOOK_LIFT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_HANDLERS_RILI_RILI_BOOK_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"

class RiliBookLiftMsgHandler : public IMcMsgHandler {
    public:
    RiliBookLiftMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_HANDLERS_RILI_RILI_BOOK_LIFT_MSG_HANDLER_H_
