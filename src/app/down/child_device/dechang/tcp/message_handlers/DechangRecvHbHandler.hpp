#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_TCP_MESSAGE_HANDLERS_DECHANG_RECV_HB_HANDLER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_TCP_MESSAGE_HANDLERS_DECHANG_RECV_HB_HANDLER_HPP_

#include "ITcpMessageHandler.hpp"
#include "DechangMessage.hpp"

class DechangRecvHbHandler : public ITcpMessageHandler {
private:
public:
    DechangRecvHbHandler();
    ~DechangRecvHbHandler();
    int handle(const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_TCP_MESSAGE_HANDLERS_DECHANG_RECV_HB_HANDLER_HPP_
