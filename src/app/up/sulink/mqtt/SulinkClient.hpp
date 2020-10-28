#ifndef APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_
#define APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_

#include "MqttClient.hpp"
#include "SulinkDeviceRegister.hpp"
#include "SulinkPacker.hpp"
#include "SulinkSendDeviceInfoHandler.hpp"
#include "SulinkRecvPassRuleInfoHandler.hpp"
#include <thread>

class SulinkClient {
private:
    MqttClient *mMqttClient;
    SulinkDeviceRegister mDeviceRegister;
    SulinkPacker mPacker;
    SulinkSendDeviceInfoHandler mSendDeviceInfoHandler;
    SulinkRecvPassRuleInfoHandler mRecvPassRuleInfoHandler;
    string mName;
    thread *mLinkThread;
    bool mIsConnected;
private:
    static void linkTask(void *rg);
public:
    SulinkClient();
    ~SulinkClient();
    int send(const Message& message);
};

#endif  //  APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_