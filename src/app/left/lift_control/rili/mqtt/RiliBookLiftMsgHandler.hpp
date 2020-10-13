#ifndef LEFT_LIFT_CONTROL_RILI_MQTT_RILI_BOOK_LIFT_MSG_HANDLER_H_
#define LEFT_LIFT_CONTROL_RILI_MQTT_RILI_BOOK_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"

class RiliBookLiftMsgHandler : public IMcMsgHandler {
    public:
    RiliBookLiftMsgHandler();
    int handle(string& topic, string& msg);
};

#endif  //  LEFT_LIFT_CONTROL_RILI_MQTT_RILI_BOOK_LIFT_MSG_HANDLER_H_
