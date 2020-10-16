#ifndef SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_
#define SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_

#include "Message.hpp"
#include <string>

using namespace std;
//基础消息
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
//心跳消息
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
//警报消息
class DechangMessageRecvWarn : public DechangMessage {
    MEMBER(unsigned char, index)
public:
    DechangMessageRecvWarn() : DechangMessage(MSG_DECHANG_RECEIVE_WARN) {
        index(0);
    }
    ~DechangMessageRecvWarn() {}
};

class DechangMessageWarnAck : public DechangMessage {
    MEMBER(unsigned char, index)
public:
    DechangMessageWarnAck() : DechangMessage(MSG_DECHANG_HB_ACK) {
        index(0);
    }
    ~DechangMessageWarnAck() {}
};
//刷卡消息
class DechangMessageRecvSwipe : public DechangMessage {
    MEMBER(string, card_no)
    MEMBER(string, time)
    MEMBER(unsigned char, type)
    MEMBER(unsigned char, door_addr)
    MEMBER(unsigned char, index)
public:
    DechangMessageRecvSwipe() : DechangMessage(MSG_DECHANG_RECEIVE_SWIPE) {
        card_no("");
        time("");
        type(0);
        door_addr(0);
        index(0);
    }
    ~DechangMessageRecvSwipe() {}
};

class DechangMessageSwipeAck : public DechangMessage {
    MEMBER(unsigned char, index)
public:
    DechangMessageSwipeAck() : DechangMessage(MSG_DECHANG_SWIPE_ACK) {
        index(0);
    }
    ~DechangMessageSwipeAck() {}
};
//增加卡消息
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