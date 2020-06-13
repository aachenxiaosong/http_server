#ifndef MSG_CENTER_SUBSCRIBER_RILI_RILI_INIT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_RILI_RILI_INIT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"

class RiliInitMsgHandler : public IMcMsgHandler {
    public:
    RiliInitMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_RILI_RILI_INIT_MSG_HANDLER_H_
