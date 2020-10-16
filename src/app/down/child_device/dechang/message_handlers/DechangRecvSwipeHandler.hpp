#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_RECV_SWIPE_HANDLER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_RECV_SWIPE_HANDLER_HPP_

#include "ITcpMessageHandler.hpp"
#include "DechangMessage.hpp"

class DechangRecvSwipeHandler : public ITcpMessageHandler {
private:
    void sendAck(const DechangMessageSwipeAck &message_ack);
public:
    DechangRecvSwipeHandler();
    ~DechangRecvSwipeHandler();
    int handle(const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_RECV_SWIPE_HANDLER_HPP_
