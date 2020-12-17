#pragma once

#include "IMqttMessageHandler.hpp"
#include "Mq.hpp"

class SulinkRecvLiftCtrlHandler : public IMqttMessageHandler {
private:
    Mq mMq;
public:
    SulinkRecvLiftCtrlHandler();
    ~SulinkRecvLiftCtrlHandler();
    int handle(const Message &message);
};