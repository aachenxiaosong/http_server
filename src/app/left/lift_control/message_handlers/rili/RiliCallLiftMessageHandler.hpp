#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class RiliCallLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    RiliCallLiftMessageHandler();
    ~RiliCallLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
