#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class RiliLiftStatusMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    RiliLiftStatusMessageHandler();
    ~RiliLiftStatusMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
