#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class RiliLiftStatusMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    RiliLiftStatusMessageHandler();
    ~RiliLiftStatusMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
