#ifndef LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_CALL_LIFT_MESSAGE_HANDLER_HPP_
#define LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_CALL_LIFT_MESSAGE_HANDLER_HPP_

#include "ILiftCtrlMessageHandler.hpp"

class WlongCallLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongCallLiftMessageHandler();
    ~WlongCallLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
#endif  //  LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_CALL_LIFT_MESSAGE_HANDLER_HPP_
