#pragma once

#include "ILiftCtrlMessageHandler.hpp"

class RiliBookLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    RiliBookLiftMessageHandler();
    ~RiliBookLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
