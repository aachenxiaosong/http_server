#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class WlongTakeLiftSimpleMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    WlongTakeLiftSimpleMessageHandler();
    ~WlongTakeLiftSimpleMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
