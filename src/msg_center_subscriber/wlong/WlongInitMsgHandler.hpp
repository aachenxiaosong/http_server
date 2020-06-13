#ifndef MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INIT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INIT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"

class WlongInitMsgHandler : public IMcMsgHandler {
    public:
    WlongInitMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INIT_MSG_HANDLER_H_
