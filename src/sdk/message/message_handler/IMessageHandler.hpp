#ifndef SDK_MESSAGE_I_MESSAGE_HANDLER_HPP_
#define SDK_MESSAGE_I_MESSAGE_HANDLER_HPP_

#include "Message.hpp"
#include <string>

using namespace std;

class IMessageHandler {
private:
    string mName;
public:
    IMessageHandler(const char *name) {mName = name;}
    virtual ~IMessageHandler() {}
    string getName() {return mName;}
    virtual int handle(const Message &message) = 0;
};

#endif  //  SDK_MESSAGE_I_MESSAGE_HANDLER_HPP_