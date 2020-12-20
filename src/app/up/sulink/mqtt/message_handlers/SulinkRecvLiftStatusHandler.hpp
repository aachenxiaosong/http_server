#pragma once

#include "IMqttMessageHandler.hpp"
#include "Mq.hpp"

class SulinkRecvLiftStatusHandler : public IMqttMessageHandler {
private:
    Mq mMq;
public:
    SulinkRecvLiftStatusHandler();
    ~SulinkRecvLiftStatusHandler();
    int handle(const Message &message);
};