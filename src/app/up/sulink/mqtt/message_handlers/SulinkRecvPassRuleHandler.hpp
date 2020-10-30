#ifndef APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_RECV_PASS_RULE_HANDLER_HPP_
#define APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_RECV_PASS_RULE_HANDLER_HPP_

#include "IMqttMessageHandler.hpp"

class SulinkRecvPassRuleHandler : public IMqttMessageHandler {
public:
    SulinkRecvPassRuleHandler();
    ~SulinkRecvPassRuleHandler();
    int handle(const Message &message);
};

#endif  //  APP_UP_SULINK_MQTT_MESSAGE_HNADLERS_SULINK_RECV_PASS_RULE_HANDLER_HPP_