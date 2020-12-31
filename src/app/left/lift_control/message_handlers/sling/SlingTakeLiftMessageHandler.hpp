#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingTakeLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingTakeLiftMessageHandler();
    ~SlingTakeLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
