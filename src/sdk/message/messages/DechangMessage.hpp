#ifndef SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_
#define SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_

#include "Message.hpp"
#include <string>

using namespace std;

class DechangMessageAddCard : public Message {
    MEMBER(string, device_id)
    MEMBER(int, user_id)
    MEMBER(string, card_no)
    MEMBER(string, password)
public:
    DechangMessageAddCard() : Message(MSG_DECHANG_ADD_CARD) {
        device_id("");
        user_id(0);
        card_no("");
        password("");
    }
    ~DechangMessageAddCard() {}
};

#endif  //  SDK_MESSAGE_MESSAGES_DECHANG_MESSAGE_HPP_