#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingBookLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingBookLiftMessageHandler();
    ~SlingBookLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
