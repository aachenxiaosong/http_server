#include "MqttClient.hpp"
#include <unistd.h>
#include "uni_log.h"

#define MQTT_CLIENT_TAG mName.c_str()

MqttClient :: MqttClient(const char *name, string server_ip, int server_port,
                         const vector<string> &publist,
                         const vector<string> &sublist)
{
    mName = name;
    mServerIp = server_ip;
    mServerPort = server_port;
    mPubList.assign(publist.begin(), publist.end());
    mSubList.assign(sublist.begin(), sublist.end());
    mPacker = NULL;
    mClient = NULL;
    mIsConnected = false;
    mReconnectThread = NULL;
}

MqttClient :: ~MqttClient() {
    if (NULL != mClient) {
        MQTTClient_disconnect(mClient, 10000);
        MQTTClient_destroy(&mClient);
        mClient = NULL;
    }
    mHandlers.clear();
    if (mReconnectThread != NULL)
    {
        mReconnectThread->join();
        delete mReconnectThread;
    }
    mPubList.clear();
    mSubList.clear();
}

int MqttClient :: connect() {
    int rc;
    string server_addr = "tcp://" + mServerIp + ":" + to_string(mServerPort);
    rc = MQTTClient_create(&mClient, server_addr.c_str(), mName.c_str(),
                           MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (MQTTCLIENT_SUCCESS != rc) {
        LOGE(MQTT_CLIENT_TAG, "mqtt create failed, rc = %d", rc);
        return -1;
    }
    /*MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(mClient, this, connlost, messageArrived, delivered);
    rc = MQTTClient_connect(mClient, &conn_opts);
    if (MQTTCLIENT_SUCCESS != rc)
    {
        LOGE(MQTT_CLIENT_TAG, "mqtt connect failed, rc = %d\n", rc);
        MQTTClient_destroy(&mClient);
        mClient = NULL;
        return -1;
    }
    LOGT(MQTT_CLIENT_TAG, "mqtt connected, server: %s", server_addr);*/
    //注册订阅
    /*for (auto topic : mSubList) {
        if (MQTTCLIENT_SUCCESS != MQTTClient_subscribe(mClient, topic.c_str(), MQTT_QOS)) {
            LOGE(MQTT_CLIENT_TAG, "subscribe for topic %s failed", topic.c_str());
        }
    }*/
    mReconnectThread = new thread(reconnectTask, this);
    return 0;
}

int MqttClient :: setPacker(IMqttPacker *packer) {
    mPacker = packer;
    return 0;
}

int MqttClient :: addHandler(IMqttMessageHandler *handler) {
    mHandlers.push_back(handler);
    return 0;
}

int MqttClient :: send(const string& pub_topic, const string& data) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token = rand();
    pubmsg.payload = (void *)data.c_str();
    pubmsg.payloadlen = data.length() + 1;
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = 0;
    int rc = MQTTClient_publishMessage(mClient, pub_topic.c_str(), &pubmsg, &token);
    LOGT(MQTT_CLIENT_TAG, "data[%d] %s is sent %s", token ,data.c_str(), MQTTCLIENT_SUCCESS == rc ? "OK" : "failed");
    if (MQTTCLIENT_SUCCESS == rc) {
        return 0;
    }
    return -1;
}

int MqttClient :: send(const Message &message) {
    int rc = -1;
    const IMqttMessage& mqtt_message = dynamic_cast<const IMqttMessage&>(message);
    string topic = findPubTopic(mqtt_message.topic());
    if (topic.empty()) {
        return rc;
    }
    string* data = mPacker->pack(mqtt_message);
    if (data) {
        rc = send(mqtt_message.topic(), *data);
        delete data;
    }
    return rc;
}

string MqttClient :: findPubTopic(const string& topic_key) {
    vector<string>::iterator it;
    for (it = mPubList.begin(); it != mPubList.end(); it++) {
        if ((*it).find(topic_key) != string::npos) {
            LOGT(MQTT_CLIENT_TAG, "topic for key %s found: %s", topic_key.c_str(), (*it).c_str());
            return (*it);
        }
    }
    LOGE(MQTT_CLIENT_TAG, "topic for key %s not found", topic_key.c_str());
    return "";
}


#define MQTT_CLIENT_TAG1 mqtt_client->mName.c_str()

void MqttClient :: reconnectTask(void *arg) {
    MqttClient *mqtt_client = (MqttClient *)arg;
    while (mqtt_client->mClient)
    {
        if (mqtt_client->mIsConnected == false)
        {
            MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
            int rc;
            conn_opts.keepAliveInterval = 20;
            conn_opts.cleansession = 1;
            rc = MQTTClient_connect(mqtt_client->mClient, &conn_opts);
            if (MQTTCLIENT_SUCCESS != rc)
            {
                LOGE(MQTT_CLIENT_TAG1, "mqtt connect failed, rc = %d\n", rc);
            } else {
                LOGT(MQTT_CLIENT_TAG1, "mqtt connected OK");
                //注册订阅
                for (auto topic : mqtt_client->mSubList)
                {
                    if (MQTTCLIENT_SUCCESS != MQTTClient_subscribe(mqtt_client->mClient, topic.c_str(), MQTT_QOS))
                    {
                        LOGE(MQTT_CLIENT_TAG1, "subscribe for topic %s failed", topic.c_str());
                    }
                }
                mqtt_client->mIsConnected = true;
            }
        }
        sleep(10);
    }
}

void MqttClient :: delivered(void *context, MQTTClient_deliveryToken dt) {
    MqttClient *mqtt_client = (MqttClient *)context;
    LOGT(MQTT_CLIENT_TAG1, "data[%d] is confirmed", dt);
}

int MqttClient :: messageArrived(void *context, char *topicName, int topicLen,
                                 MQTTClient_message *message) {
    int message_handled_ok = -1;
    MqttClient *mqtt_client = (MqttClient *)context;
    IMqttMessageHandler *i_handler = NULL;
    if (mqtt_client->mPacker) {
        string data((const char *)message->payload, message->payloadlen);
        IMqttMessage *mqtt_message = mqtt_client->mPacker->unpack(data);
        if (mqtt_message)
        {
            mqtt_message->topic(topicName);
            for (vector<IMqttMessageHandler *>::iterator it = mqtt_client->mHandlers.begin(); it != mqtt_client->mHandlers.end(); it++)
            {
                i_handler = *it;
                if (i_handler->handle(*mqtt_message) == 0)
                {
                    message_handled_ok = 0;
                    break;
                }
            }
            delete mqtt_message;
        }
    }
    return message_handled_ok;
}

void MqttClient :: connlost(void *context, char *cause) {
    MqttClient *mqtt_client = (MqttClient *)context;
    mqtt_client->mIsConnected = false;
    LOGT(MQTT_CLIENT_TAG1, "mqtt connection lost");
}