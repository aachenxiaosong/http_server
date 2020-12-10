#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class RiliTakeLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    RiliTakeLiftMessageHandler();
    ~RiliTakeLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
