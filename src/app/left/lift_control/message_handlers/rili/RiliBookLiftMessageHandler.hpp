#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class RiliBookLiftMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    RiliBookLiftMessageHandler();
    ~RiliBookLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
