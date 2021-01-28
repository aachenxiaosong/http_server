#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class WlongBookLiftSimpleMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    WlongBookLiftSimpleMessageHandler();
    ~WlongBookLiftSimpleMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
