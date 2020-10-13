#ifndef LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_CALL_LIFT_MSG_HANDLER_H_
#define LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_CALL_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"

using namespace neb;


class WlongCallLiftMsgHandler : public IMcMsgHandler {
    private:
    int _checkCallLift(CJsonObject& msg, string& err_field) ;
    int _handleCallLift(CJsonObject& msg, CJsonObject& jresponse) ;
    public:
    WlongCallLiftMsgHandler();
    int handle(string& topic, string& msg);
};

#endif  //  LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_CALL_LIFT_MSG_HANDLER_H_
