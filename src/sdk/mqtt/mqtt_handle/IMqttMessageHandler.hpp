#ifndef  SDK_MQTT_MQTT_HANDLE_I_MQTT_MESSAGE_HANDLER_HPP_
#define  SDK_MQTT_MQTT_HANDLE_I_MQTT_MESSAGE_HANDLER_HPP_

#include "IMessageHandler.hpp"
#include <string>

using namespace std;

class MqttClient;

class IMqttMessageHandler : public IMessageHandler {
    protected:
    MqttClient *mConn;
    public:
    IMqttMessageHandler(const char* name) : IMessageHandler(name) {}
    virtual ~IMqttMessageHandler() {}
    void setConn(MqttClient *conn) { mConn = conn; }
    virtual int handle(const Message &message) = 0;
};

#endif  //   SDK_MQTT_MQTT_HANDLE_I_MQTT_MESSAGE_HANDLER_HPP_