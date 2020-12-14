#ifndef  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_HPP_
#define  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_HPP_

#include "IMqttMessageHandler.hpp"
#include "IMqttPacker.hpp"
#include "MQTTClient.h"
#include "MqttClientParam.hpp"
#include <string>
#include <vector>
#include <thread>

#define MQTT_RECV_MAX_LEN (1024 * 100)
#define MQTT_QOS          1

using namespace std;

class MqttClient {
private:
    string mName;   
    MqttClientParam mParam;
    MQTTClient mClient;
    IMqttPacker *mPacker;
    vector <IMqttMessageHandler *> mHandlers;

    bool mIsConnected;
    thread* mReconnectThread;

public:
    MqttClient(const char *name, const MqttClientParam& param);
    ~MqttClient();
    int connect();
    int setPacker(IMqttPacker *packer);
    int addHandler(IMqttMessageHandler *handler);
    int send(const string& pub_topic, const string& data);
    int send(const Message& message);
    //int onRecv(const Message& message);
private:
    string findPubTopic(const string& topic_key);

private:
    static void reconnectTask(void *arg);
    static void delivered(void *context, MQTTClient_deliveryToken dt);
    static int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message);
    static void connlost(void *context, char *cause);
};
#endif  //  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_HPP_