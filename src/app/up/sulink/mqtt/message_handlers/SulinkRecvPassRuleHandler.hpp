#pragma once

#include "IMqttMessageHandler.hpp"

class SulinkRecvPassRuleHandler : public IMqttMessageHandler {
public:
    SulinkRecvPassRuleHandler();
    ~SulinkRecvPassRuleHandler();
    int handle(const Message &message);
};