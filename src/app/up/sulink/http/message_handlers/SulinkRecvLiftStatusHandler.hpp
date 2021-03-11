#pragma once

#include "ISulinkMessageHandler.hpp"
#include "Mq.hpp"

class SulinkRecvLiftStatusHandler : public ISulinkMessageHandler {
private:
    Mq mMq;
public:
    SulinkRecvLiftStatusHandler();
    ~SulinkRecvLiftStatusHandler();
    Message * handle(const Message &message);
};