#ifndef SERVER_LIFT_CONTROL_RILI_MQTT_RILI_BOOK_LIFT_MSG_HANDLER_H_
#define SERVER_LIFT_CONTROL_RILI_MQTT_RILI_BOOK_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"

class RiliBookLiftMsgHandler : public IMcMsgHandler {
    public:
    RiliBookLiftMsgHandler();
    int handle(string& msg);
};

#endif  //  SERVER_LIFT_CONTROL_RILI_MQTT_RILI_BOOK_LIFT_MSG_HANDLER_H_
