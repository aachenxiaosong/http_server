#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingBookLiftInterMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingBookLiftInterMessageHandler();
    ~SlingBookLiftInterMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
