#pragma once

#include "IMqttMessageHandler.hpp"

class SulinkRecvLiftInfoHandler : public IMqttMessageHandler {
public:
    SulinkRecvLiftInfoHandler();
    ~SulinkRecvLiftInfoHandler();
    int handle(const Message &message);
};
