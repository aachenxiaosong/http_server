#pragma once

#include "ISulinkMessageHandler.hpp"

class SulinkRecvLiftInfoHandler : public ISulinkMessageHandler {
public:
    SulinkRecvLiftInfoHandler();
    ~SulinkRecvLiftInfoHandler();
    Message * handle(const Message &message);
};
