#pragma once
#include "IMqttMessageHandler.hpp"

class SulinkSendDeviceInfoHandler : public IMqttMessageHandler {
public:
    SulinkSendDeviceInfoHandler();
    ~SulinkSendDeviceInfoHandler();
    int handle(const Message &message);
};
