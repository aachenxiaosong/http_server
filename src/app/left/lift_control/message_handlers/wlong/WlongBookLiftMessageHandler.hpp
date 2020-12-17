#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class WlongBookLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongBookLiftMessageHandler();
    ~WlongBookLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
