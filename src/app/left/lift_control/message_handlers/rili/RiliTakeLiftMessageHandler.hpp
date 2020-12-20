#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class RiliTakeLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    RiliTakeLiftMessageHandler();
    ~RiliTakeLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
