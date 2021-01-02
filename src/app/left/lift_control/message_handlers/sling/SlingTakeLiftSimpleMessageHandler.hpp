#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingTakeLiftSimpleMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingTakeLiftSimpleMessageHandler();
    ~SlingTakeLiftSimpleMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
