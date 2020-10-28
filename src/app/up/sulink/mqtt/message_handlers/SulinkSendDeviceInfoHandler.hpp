#ifndef APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_SEND_DEVICE_INFO_HANDLER_HPP_
#define APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_SEND_DEVICE_INFO_HANDLER_HPP_

#include "IMqttMessageHandler.hpp"

class SulinkSendDeviceInfoHandler : public IMqttMessageHandler {
public:
    SulinkSendDeviceInfoHandler();
    ~SulinkSendDeviceInfoHandler();
    int handle(const Message &message);
};
#endif  //  APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_SEND_DEVICE_INFO_HANDLER_HPP_
