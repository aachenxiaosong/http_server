#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class WlongLiftStatusMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    WlongLiftStatusMessageHandler();
    ~WlongLiftStatusMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
