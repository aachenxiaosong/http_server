#ifndef APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_
#define APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_

#include "MqttClient.hpp"
#include "SulinkDeviceRegister.hpp"
#include "SulinkPacker.hpp"
#include "SulinkSendDeviceInfoHandler.hpp"
#include "SulinkSendPassRecordHandler.hpp"
#include "SulinkRecvPassRuleHandler.hpp"
#include "SulinkRecvLiftInfoHandler.hpp"
#include "SulinkRecvLiftCtrlHandler.hpp"
#include "SulinkRecvLiftStatusHandler.hpp"
#include "Mq.hpp"
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
    SulinkRecvLiftCtrlHandler mRecvLiftCtrlHandler;
    SulinkRecvLiftStatusHandler mRecvLiftStatusHandler;
    string mName;
    thread *mLinkThread;
    bool mIsConnected;
    Mq *mMq;
    thread *mMqThread;
    bool mMqThreadRunning;
private:
    static void linkTask(void *rg);
    static void mqRecvTask(void *arg);
public:
    SulinkClient();
    ~SulinkClient();
    int start();
    int send(const Message& message);
    //int onRecvMessage(const Message& message);
};

#endif  //  APP_UP_SULINK_MQTT_SULINK_CLIENT_HPP_
