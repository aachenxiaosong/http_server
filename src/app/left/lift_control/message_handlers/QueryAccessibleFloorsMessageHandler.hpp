#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"

class QueryAccessibleFloorsMessageHandler : public ILiftCtrlHttpMessageHandler {
private:
public:
    QueryAccessibleFloorsMessageHandler();
    ~QueryAccessibleFloorsMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
