#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class WlongCallLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    WlongCallLiftMessageHandler();
    ~WlongCallLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
