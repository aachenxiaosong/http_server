#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class WlongLiftStatusMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongLiftStatusMessageHandler();
    ~WlongLiftStatusMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
