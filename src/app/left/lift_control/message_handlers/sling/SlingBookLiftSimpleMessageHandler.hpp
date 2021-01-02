#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class SlingBookLiftSimpleMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    SlingBookLiftSimpleMessageHandler();
    ~SlingBookLiftSimpleMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
