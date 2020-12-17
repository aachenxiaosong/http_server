#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class WlongBookLiftInterMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongBookLiftInterMessageHandler();
    ~WlongBookLiftInterMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
