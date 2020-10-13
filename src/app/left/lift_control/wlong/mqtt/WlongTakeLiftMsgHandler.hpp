#ifndef LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_TAKE_LIFT_MSG_HANDLER_H_
#define LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_TAKE_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"

using namespace neb;


class WlongTakeLiftMsgHandler : public IMcMsgHandler {
    private:
    int _checkTakeLift(CJsonObject& msg, string& err_field) ;
    int _handleTakeLift(CJsonObject& msg, CJsonObject& jresponse) ;
    public:
    WlongTakeLiftMsgHandler();
    int handle(string& topic, string& msg);
};

#endif  //  LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_TAKE_LIFT_MSG_HANDLER_H_
