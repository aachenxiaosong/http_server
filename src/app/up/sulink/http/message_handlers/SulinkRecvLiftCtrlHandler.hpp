#pragma once

#include "ISulinkMessageHandler.hpp"
#include "Mq.hpp"

class SulinkRecvLiftCtrlHandler : public ISulinkMessageHandler {
private:
    Mq mMq;
public:
    SulinkRecvLiftCtrlHandler();
    ~SulinkRecvLiftCtrlHandler();
    Message * handle(const Message &message);
};