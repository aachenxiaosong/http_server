#ifndef APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_RECV_LIFT_INFO_HANDLER_HPP_
#define APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_RECV_LIFT_INFO_HANDLER_HPP_

#include "IMqttMessageHandler.hpp"

class SulinkRecvLiftInfoHandler : public IMqttMessageHandler {
public:
    SulinkRecvLiftInfoHandler();
    ~SulinkRecvLiftInfoHandler();
    int handle(const Message &message);
};

#endif  //  APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_RECV_LIFT_INFO_HANDLER_HPP_