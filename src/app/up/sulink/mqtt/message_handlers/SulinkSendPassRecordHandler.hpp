#ifndef APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_SEND_PASS_RECORD_HANDLER_HPP_
#define APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_SEND_PASS_RECORD_HANDLER_HPP_

#include "IMqttMessageHandler.hpp"

class SulinkSendPassRecordHandler : public IMqttMessageHandler {
private:
    bool mAckReceived;
public:
    SulinkSendPassRecordHandler();
    ~SulinkSendPassRecordHandler();
    int handle(const Message &message);
};
#endif  //  APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_SEND_PASS_RECORD_HANDLER_HPP_
