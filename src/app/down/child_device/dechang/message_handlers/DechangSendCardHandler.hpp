#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_CARD_HANDLER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_CARD_HANDLER_HPP_

#include "ITcpMessageHandler.hpp"
#include "DechangMessage.hpp"

class DechangSendCardHandler : public ITcpMessageHandler {
private:
    int mAckReceived;
public:
    DechangSendCardHandler();
    ~DechangSendCardHandler();
    int handle(const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_CARD_HANDLER_HPP_
