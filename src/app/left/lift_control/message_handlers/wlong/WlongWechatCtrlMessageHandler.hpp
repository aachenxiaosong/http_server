#pragma once

#include "ILiftCtrlMqMessageHandler.hpp"

class WlongWechatCtrlMessageHandler : public ILiftCtrlMqMessageHandler {
private:
public:
    WlongWechatCtrlMessageHandler();
    ~WlongWechatCtrlMessageHandler();
    int handle(const LiftCtrlMessageReq &request);
};
