#pragma once

#include "ILiftCtrlMqMessageHandler.hpp"

class WlongWechatStatusMessageHandler : public ILiftCtrlMqMessageHandler {
private:
public:
    WlongWechatStatusMessageHandler();
    ~WlongWechatStatusMessageHandler();
    int handle(const LiftCtrlMessageReq &request);
};
