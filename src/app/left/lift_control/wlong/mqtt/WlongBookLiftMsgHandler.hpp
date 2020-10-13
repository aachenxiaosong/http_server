#ifndef LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_BOOK_LIFT_MSG_HANDLER_H_
#define LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_BOOK_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"

using namespace neb;


class WlongBookLiftMsgHandler : public IMcMsgHandler {
    private:
    int _checkBookLift(CJsonObject& msg, string& err_field) ;
    int _handleBookLift(CJsonObject& msg, CJsonObject& jresponse) ;
    public:
    WlongBookLiftMsgHandler();
    int handle(string& topic, string& msg);
};

#endif  //  LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_BOOK_LIFT_MSG_HANDLER_H_
