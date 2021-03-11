#ifndef  SDK_MQTT_MQTT_HANDLE_I_MQTT_MESSAGE_HANDLER_HPP_
#define  SDK_MQTT_MQTT_HANDLE_I_MQTT_MESSAGE_HANDLER_HPP_

#include "IMessageHandler.hpp"
#include <string>

using namespace std;


class ISulinkMessageHandler : public IMessageHandler {
protected:

public:
    ISulinkMessageHandler(const char* name) : IMessageHandler(name) {

    }
    virtual ~ISulinkMessageHandler() {}
    virtual Message * handle(const Message &message) = 0;
};

#endif  //   SDK_MQTT_MQTT_HANDLE_I_MQTT_MESSAGE_HANDLER_HPP_