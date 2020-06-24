#ifndef MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_CALL_LIFT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_CALL_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"

using namespace neb;


class WlongCallLiftMsgHandler : public IMcMsgHandler {
    private:
    int _checkCallLift(CJsonObject& msg, string& err_field) ;
    int _handleCallLift(CJsonObject& msg, CJsonObject& jresponse) ;
    public:
    WlongCallLiftMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_CALL_LIFT_MSG_HANDLER_H_
