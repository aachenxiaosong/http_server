#ifndef  SDK_SULINK_HANDLE_IMQTT_MESSAGE_HPP_
#define  SDK_SULINK_HANDLE_IMQTT_MESSAGE_HPP_

#include <string>
#include <vector>
#include "Message.hpp"

using namespace std;


class ISulinkMessage : public Message {
    MEMBER(string, topic)
    SERI_BEGIN
    SERI(topic)
    SERI_END
public:
    ISulinkMessage(MessageType type) : Message(type) {}
};

#endif  //  SDK_SULINK_HANDLE_IMQTT_MESSAGE_HPP_