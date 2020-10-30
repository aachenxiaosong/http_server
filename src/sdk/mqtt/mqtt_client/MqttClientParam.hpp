#ifndef  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_PARAM_HPP_
#define SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_PARAM_HPP_

#include <string>
#include <vector>

using namespace std;

struct MqttClientParam {
    string clientId;
    string serverIp;
    int serverPort;
    string userName;
    string password;
    int keepAlive;
    vector<string> subTopics;
    vector<string> pubTopics;
    MqttClientParam() {
        serverPort = 0;
        keepAlive = 0;
    }
    MqttClientParam(const MqttClientParam& param) {
        clientId = param.clientId;
        serverIp = param.serverIp;
        serverPort = param.serverPort;
        userName = param.userName;
        password = param.password;
        keepAlive = param.keepAlive;
        subTopics.assign(param.subTopics.begin(), param.subTopics.end());
        pubTopics.assign(param.pubTopics.begin(), param.pubTopics.end());
    }
    ~MqttClientParam() {
        subTopics.clear();
        pubTopics.clear();
    }
    
    MqttClientParam& operator=(const MqttClientParam& param) {
        subTopics.clear();
        pubTopics.clear();
        clientId = param.clientId;
        serverIp = param.serverIp;
        serverPort = param.serverPort;
        userName = param.userName;
        password = param.password;
        keepAlive = param.keepAlive;
        subTopics.assign(param.subTopics.begin(), param.subTopics.end());
        pubTopics.assign(param.pubTopics.begin(), param.pubTopics.end());
        return *this;
    }

    string toString() const
    {
        string s;
        s.append("--------------------------- sulink mqtt info begin --------------------------");
        s.append("\nclientId: " + clientId);
        s.append("\nserverIp: " + serverIp);
        s.append("\nserverPort: " + to_string(serverPort));
        s.append("\nuserName: " + userName);
        s.append("\npassWord: " + password);
        s.append("\nkeepAlive: " + to_string(keepAlive));
        s.append("\npubTopics:");
        for (int i = 0; i < pubTopics.size(); i++)
        {
            s.append("\n  " + subTopics[i]);
        }
        s.append("\nsubTopics:");
        for (int i = 0; i < subTopics.size(); i++)
        {
            s.append("\n  " + subTopics[i]);
        }
        s.append("\n--------------------------- sulink mqtt info end --------------------------");
        return s;
    }
};

#endif  //  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_PARAM_HPP_