#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class WlongTakeLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongTakeLiftMessageHandler();
    ~WlongTakeLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
