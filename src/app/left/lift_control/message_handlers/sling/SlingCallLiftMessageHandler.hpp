#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingCallLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingCallLiftMessageHandler();
    ~SlingCallLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
