#ifndef MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_TAKE_LIFT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_TAKE_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"

using namespace neb;


class WlongTakeLiftMsgHandler : public IMcMsgHandler {
    private:
    int _checkTakeLift(CJsonObject& msg, string& err_field) ;
    int _handleTakeLift(CJsonObject& msg, CJsonObject& jresponse) ;
    public:
    WlongTakeLiftMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_TAKE_LIFT_MSG_HANDLER_H_
