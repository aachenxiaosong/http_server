#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class WlongTakeLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    WlongTakeLiftMessageHandler();
    ~WlongTakeLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
