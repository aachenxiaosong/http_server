#ifndef LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_BOOK_LIFT_INTER_MESSAGE_HANDLER_HPP_
#define LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_BOOK_LIFT_INTER_MESSAGE_HANDLER_HPP_

#include "ILiftCtrlMessageHandler.hpp"

class WlongBookLiftInterMessageHandler : public ILiftCtrlMessageHandler {
private:
public:
    WlongBookLiftInterMessageHandler();
    ~WlongBookLiftInterMessageHandler();
    LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request);
};
#endif  //  LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_WLONG_WLONG_BOOK_LIFT_INTER_MESSAGE_HANDLER_HPP_
