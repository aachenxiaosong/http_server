#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class WlongBookLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    WlongBookLiftMessageHandler();
    ~WlongBookLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
