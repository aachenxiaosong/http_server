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
class DechangMessageRecvHbAck : public DechangMessage {
    MEMBER(unsigned char, customer_code_h)
    MEMBER(unsigned char, customer_code_l)
public:
    DechangMessageRecvHbAck() : DechangMessage(MSG_DECHANG_RECEIVE_HB_ACK) {
        customer_code_h(0);
        customer_code_l(0);
    }
    ~DechangMessageRecvHbAck() {}
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

class DechangMessageRecvWarnAck : public DechangMessage {
    MEMBER(unsigned char, index)
public:
    DechangMessageRecvWarnAck() : DechangMessage(MSG_DECHANG_RECEIVE_WARN_ACK) {
        index(0);
    }
    ~DechangMessageRecvWarnAck() {}
};
//刷卡消息
class DechangMessageRecvSwipe : public DechangMessage {
    MEMBER(unsigned long, card_no)
    MEMBER(string, time)
    MEMBER(unsigned char, type)
    MEMBER(unsigned char, door_addr)
    MEMBER(unsigned char, index)
public:
    DechangMessageRecvSwipe() : DechangMessage(MSG_DECHANG_RECEIVE_SWIPE) {
        card_no(0);
        time("");
        type(0);
        door_addr(0);
        index(0);
    }
    ~DechangMessageRecvSwipe() {}
};

class DechangMessageRecvSwipeAck : public DechangMessage {
    MEMBER(unsigned char, index)
public:
    DechangMessageRecvSwipeAck() : DechangMessage(MSG_DECHANG_RECEIVE_SWIPE_ACK) {
        index(0);
    }
    ~DechangMessageRecvSwipeAck() {}
};
//增加卡消息
//device_id是刷卡其的序列号,用于唯一确定tcp长连接
//user_id需要从0开始,比如宿舍4个人,应为0 1 2 3
//user_name必填,固定8字节,不够补0
//卡号4字节,需要按要求转换
//密码,固定8字节
//超时时间6字节,需要按要求转换
//状态,1表示有效 0表示无效
class DechangMessageSendCard : public DechangMessage {
    MEMBER(string, device_id)
    MEMBER(unsigned int, user_id)
    MEMBER(string, user_name)
    MEMBER(unsigned long, card_no)
    MEMBER(string, password)
    MEMBER(unsigned short, door_access)
    MEMBER(string, expire_date)
    MEMBER(unsigned char, status)
public:
    DechangMessageSendCard() : DechangMessage(MSG_DECHANG_SEND_CARD) {
        device_id("");
        user_id(0);
        user_name("");
        card_no(0);
        password("");
        door_access(0);
        expire_date("");
        status(0);
    }
    ~DechangMessageSendCard() {}
};

class DechangMessageSendCardAck : public DechangMessage {
    MEMBER(unsigned char, ack)
public:
    DechangMessageSendCardAck() : DechangMessage(MSG_DECHANG_SEND_CARD_ACK) {
        ack(0);
    }
    ~DechangMessageSendCardAck() {}
};

#endif  //  SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_