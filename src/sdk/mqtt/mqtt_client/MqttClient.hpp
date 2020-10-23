#ifndef  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_HPP_
#define  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_HPP_

#include "IMqttMessageHandler.hpp"
#include "IMqttPacker.hpp"
#include <thread>
#include <string>
#include <vector>

#define MAX_MQTT_RECV_LEN 1024 * 100

using namespace std;

class MqttClient {
private:
    string mName;
    string mServerIp;
    int mServerPort;
    vector<string> mPubList;
    vector<string> mSubList;

    struct event_base *mEventBase;
    thread *mThread;
    
    IMqttPacker *mPacker;
    vector <IMqttMessageHandler *> mHandlers;

public:
    MqttClient(const char *name, string server_ip, int server_port);
    ~MqttClient();
    int connect();
    int setPacker(IMqttPacker *packer);
    int addHandler(IMqttMessageHandler *handler);
    int send(const char *data, int len);
    int send(const Message& message);

private:
    static void readCb(struct bufferevent *bev, void *arg);
    static void eventCb(struct bufferevent *bev, short events, void *arg);
    static void dispathTask(void *arg);
};
#endif  //  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_HPP_