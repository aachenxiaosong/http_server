#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_RECV_WARN_HANDLER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_RECV_WARN_HANDLER_HPP_

#include "ITcpMessageHandler.hpp"
#include "DechangMessage.hpp"

class DechangRecvWarnHandler : public ITcpMessageHandler {
private:
public:
    DechangRecvWarnHandler();
    ~DechangRecvWarnHandler();
    int handle(const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_RECV_WARN_HANDLER_HPP_
