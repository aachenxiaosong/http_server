#ifndef LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_TAKE_LIFT_MESSAGE_HANDLER_HPP_
#define LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_TAKE_LIFT_MESSAGE_HANDLER_HPP_

#include "ILiftCtrlMessageHandler.hpp"

class WlongTakeLiftMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongTakeLiftMessageHandler();
    ~WlongTakeLiftMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
#endif  //  LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_TAKE_LIFT_MESSAGE_HANDLER_HPP_
