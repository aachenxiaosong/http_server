#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingshBookLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingshBookLiftMessageHandler();
    ~SlingshBookLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
