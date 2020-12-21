#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingshCallLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingshCallLiftMessageHandler();
    ~SlingshCallLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
