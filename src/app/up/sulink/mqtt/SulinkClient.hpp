#ifndef APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_
#define APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_

#include "MqttClient.hpp"
#include "SulinkDeviceRegister.hpp"
#include "SulinkPacker.hpp"
#include "SulinkSendDeviceInfoHandler.hpp"
#include "SulinkSendPassRecordHandler.hpp"
#include "SulinkRecvPassRuleHandler.hpp"
#include "SulinkRecvLiftInfoHandler.hpp"
#include <thread>

class SulinkClient {
private:
    MqttClient *mMqttClient;
    SulinkDeviceRegister mDeviceRegister;
    SulinkPacker mPacker;
    SulinkSendDeviceInfoHandler mSendDeviceInfoHandler;
    SulinkSendPassRecordHandler mSendPassRecordHandler;
    SulinkRecvPassRuleHandler mRecvPassRuleHandler;
    SulinkRecvLiftInfoHandler mRecvLiftInfoHandler;
    string mName;
    thread *mLinkThread;
    bool mIsConnected;
private:
    static void linkTask(void *rg);
public:
    SulinkClient();
    ~SulinkClient();
    int start();
    //int onRecvMessage(const Message& message);
};

#endif  //  APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_
