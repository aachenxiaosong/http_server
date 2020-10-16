#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_ADD_CARD_HANDLER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_ADD_CARD_HANDLER_HPP_

#include "ITcpMessageHandler.hpp"
#include "DechangMessage.hpp"

class DechangSendAddCardHandler : public ITcpMessageHandler {
private:
    int mAckReceived;
public:
    DechangSendAddCardHandler();
    ~DechangSendAddCardHandler();
    int handle(const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_ADD_CARD_HANDLER_HPP_
