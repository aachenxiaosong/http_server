#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class WlongBookLiftInterMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    WlongBookLiftInterMessageHandler();
    ~WlongBookLiftInterMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
