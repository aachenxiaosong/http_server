#ifndef SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_
#define SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_

#include "Message.hpp"
#include <string>

using namespace std;

class DechangMessage : public Message {
    MEMBER(unsigned char, rand)
    MEMBER(unsigned char, cmd)
    MEMBER(unsigned char, address)
    MEMBER(unsigned char, door)
public:
    DechangMessage(MessageType type) : Message(type) {
        cmd(0);
        door(0);
    }
    virtual ~DechangMessage() {}
};

class DechangMessageRecvHb : public DechangMessage {
    MEMBER(string, device_id)
    MEMBER(int, status)
public:
    DechangMessageRecvHb() : DechangMessage(MSG_DECHANG_RECEIVE_HB) {
        device_id("");
        status(0);
    }
    ~DechangMessageRecvHb() {}
};

class DechangMessageHbAck : public DechangMessage {
    MEMBER(unsigned char, customer_code_h)
    MEMBER(unsigned char, customer_code_l)
public:
    DechangMessageHbAck() : DechangMessage(MSG_DECHANG_HB_ACK) {
        customer_code_h(0);
        customer_code_l(0);
    }
    ~DechangMessageHbAck() {}
};

class DechangMessageSendCard : public DechangMessage {
    MEMBER(string, device_id)
    MEMBER(int, user_id)
    MEMBER(string, card_no)
    MEMBER(string, password)
public:
    DechangMessageSendCard() : DechangMessage(MSG_DECHANG_SEND_CARD) {
        device_id("");
        user_id(0);
        card_no("");
        password("");
    }
    ~DechangMessageSendCard() {}
};

#endif  //  SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_