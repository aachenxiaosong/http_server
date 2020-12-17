#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class RiliCallLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    RiliCallLiftMessageHandler();
    ~RiliCallLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
