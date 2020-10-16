#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_DEL_CARD_HANDLER_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_DEL_CARD_HANDLER_HPP_

#include "ITcpMessageHandler.hpp"
#include "DechangMessage.hpp"

class DechangSendDelCardHandler : public ITcpMessageHandler {
private:
    int mAckReceived;
public:
    DechangSendDelCardHandler();
    ~DechangSendDelCardHandler();
    int handle(const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_MESSAGE_HANDLERS_DECHANG_SEND_DEL_CARD_HANDLER_HPP_
