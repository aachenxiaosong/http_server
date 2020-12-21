#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingshBookLiftInterMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingshBookLiftInterMessageHandler();
    ~SlingshBookLiftInterMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
