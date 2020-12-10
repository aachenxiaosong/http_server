#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class RiliBookLiftInterMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    RiliBookLiftInterMessageHandler();
    ~RiliBookLiftInterMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
