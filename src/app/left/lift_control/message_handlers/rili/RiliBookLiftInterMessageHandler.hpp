#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class RiliBookLiftInterMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    RiliBookLiftInterMessageHandler();
    ~RiliBookLiftInterMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
