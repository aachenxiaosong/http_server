#include "MqttClient.hpp"
#include <unistd.h>
#include "UniLog.hpp"

#define MQTT_CLIENT_TAG mName.c_str()

MqttClient :: MqttClient(const char *name, const MqttClientParam& param)
{
    mName = name;
    mParam = param;
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
}

int MqttClient :: connect() {
    int rc;
    string server_addr = "tcp://" + mParam.serverIp + ":" + to_string(mParam.serverPort);
    rc = MQTTClient_create(&mClient, server_addr.c_str(), mParam.clientId.c_str(),
                           MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (MQTTCLIENT_SUCCESS != rc) {
        LOGE(MQTT_CLIENT_TAG, "mqtt create failed, rc = %d", rc);
        return -1;
    }
    LOGT(MQTT_CLIENT_TAG, "mqtt create ok, server=%s client_id=%s", server_addr.c_str(), mParam.clientId.c_str());
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
        rc = send(topic, *data);
        delete data;
    }
    return rc;
}

int MqttClient :: onRecv(const Message& message) {
    int message_handled_ok = -1;
    IMqttMessageHandler *i_handler = NULL;
    vector<IMqttMessageHandler *>::iterator it;
    for (it = mHandlers.begin(); it != mHandlers.end(); it++)
    {
        i_handler = *it;
        if (i_handler->handle(message) == 0)
        {
            message_handled_ok = 0;
            break;
        }
    }
    return message_handled_ok;
}

string MqttClient :: findPubTopic(const string& topic_key) {
    vector<string>::iterator it;
    for (it = mParam.pubTopics.begin(); it != mParam.pubTopics.end(); it++) {
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
            conn_opts.keepAliveInterval = mqtt_client->mParam.keepAlive;
            conn_opts.cleansession = 1;
            conn_opts.username = mqtt_client->mParam.userName.c_str();
            conn_opts.password = mqtt_client->mParam.password.c_str();
            MQTTClient_setCallbacks(mqtt_client->mClient, mqtt_client, connlost, messageArrived, delivered);
            LOGT(MQTT_CLIENT_TAG1, "mqtt connecting... username:%s password:%s", conn_opts.username, conn_opts.password);
            rc = MQTTClient_connect(mqtt_client->mClient, &conn_opts);
            if (MQTTCLIENT_SUCCESS != rc)
            {
                LOGE(MQTT_CLIENT_TAG1, "mqtt connect failed, rc = %d\n", rc);
            } else {
                LOGT(MQTT_CLIENT_TAG1, "mqtt connected OK");
                //注册订阅
                for (auto topic : mqtt_client->mParam.subTopics)
                {
                    if (MQTTCLIENT_SUCCESS != MQTTClient_subscribe(mqtt_client->mClient, topic.c_str(), MQTT_QOS))
                    {
                        LOGE(MQTT_CLIENT_TAG1, "subscribe for topic %s failed", topic.c_str());
                    } else {
                        LOGT(MQTT_CLIENT_TAG1, "subscribe for topic %s OK", topic.c_str());
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
    //int message_handled_ok = -1;
    MqttClient *mqtt_client = (MqttClient *)context;
    IMqttMessageHandler *i_handler = NULL;
    string sdata((const char *)message->payload, message->payloadlen);
    LOGT(MQTT_CLIENT_TAG1, "message arrived, topic:%s data:%s", topicName, sdata.c_str());
    if (mqtt_client->mPacker) {
        IMqttMessage *mqtt_message = mqtt_client->mPacker->unpack(sdata);
        if (mqtt_message)
        {
            mqtt_message->topic(topicName);
            for (vector<IMqttMessageHandler *>::iterator it = mqtt_client->mHandlers.begin(); it != mqtt_client->mHandlers.end(); it++)
            {
                i_handler = *it;
                if (i_handler->handle(*mqtt_message) == 0)
                {
                    //message_handled_ok = 0;
                    break;
                }
            }
            delete mqtt_message;
        }
    }
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    //return message_handled_ok;
    //成功处理了,必须free掉message和topicName,并且返回1
    return 1;
}

void MqttClient :: connlost(void *context, char *cause) {
    MqttClient *mqtt_client = (MqttClient *)context;
    mqtt_client->mIsConnected = false;
    LOGT(MQTT_CLIENT_TAG1, "mqtt connection lost");
}
