#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_TCP_MESSAGE_HANDLERS_DECHANG_RECV_SWIPE_HANDLER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_TCP_MESSAGE_HANDLERS_DECHANG_RECV_SWIPE_HANDLER_HPP_

#include "ITcpMessageHandler.hpp"
#include "DechangMessage.hpp"

class DechangRecvSwipeHandler : public ITcpMessageHandler {
private:
public:
    DechangRecvSwipeHandler();
    ~DechangRecvSwipeHandler();
    int handle(const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_TCP_MESSAGE_HANDLERS_DECHANG_RECV_SWIPE_HANDLER_HPP_
