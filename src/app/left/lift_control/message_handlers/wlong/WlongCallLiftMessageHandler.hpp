#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class WlongCallLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongCallLiftMessageHandler();
    ~WlongCallLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
