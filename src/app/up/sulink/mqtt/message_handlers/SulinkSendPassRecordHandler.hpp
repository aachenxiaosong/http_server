#pragma once

#include "IMqttMessageHandler.hpp"

class SulinkSendPassRecordHandler : public IMqttMessageHandler {
private:
    bool mAckReceived;
public:
    SulinkSendPassRecordHandler();
    ~SulinkSendPassRecordHandler();
    int handle(const Message &message);
};
