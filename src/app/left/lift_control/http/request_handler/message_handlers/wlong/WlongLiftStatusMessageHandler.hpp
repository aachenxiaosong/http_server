#ifndef LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_LIFT_STATUS_MESSAGE_HANDLER_HPP_
#define LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_LIFT_STATUS_MESSAGE_HANDLER_HPP_

#include "ILiftCtrlMessageHandler.hpp"

class WlongLiftStatusMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongLiftStatusMessageHandler();
    ~WlongLiftStatusMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
#endif  //  LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_LIFT_STATUS_MESSAGE_HANDLER_HPP_
