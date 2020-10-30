#ifndef  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_PARAM_HPP_
#define SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_PARAM_HPP_

#include <string>
#include <vector>

using namespace std;

struct MqttClientParam {
    string mClientId;
    string mServerIp;
    int mServerPort;
    string mUserName;
    string mPassword;
    int mKeepAlive;
    vector<string> mSubTopics;
    vector<string> mPubTopics;
    MqttClientParam() {
        mServerPort = 0;
        mKeepAlive = 0;
    }
    MqttClientParam(const MqttClientParam& param) {
        mClientId = param.mClientId;
        mServerIp = param.mServerIp;
        mServerPort = param.mServerPort;
        mUserName = param.mUserName;
        mPassword = param.mPassword;
        mKeepAlive = param.mKeepAlive;
        mSubTopics.assign(param.mSubTopics.begin(), param.mSubTopics.end());
        mPubTopics.assign(param.mPubTopics.begin(), param.mPubTopics.end());
    }
    ~MqttClientParam() {
        mSubTopics.clear();
        mPubTopics.clear();
    }
    
    MqttClientParam& operator=(const MqttClientParam& param) {
        mSubTopics.clear();
        mPubTopics.clear();
        mClientId = param.mClientId;
        mServerIp = param.mServerIp;
        mServerPort = param.mServerPort;
        mUserName = param.mUserName;
        mPassword = param.mPassword;
        mKeepAlive = param.mKeepAlive;
        mSubTopics.assign(param.mSubTopics.begin(), param.mSubTopics.end());
        mPubTopics.assign(param.mPubTopics.begin(), param.mPubTopics.end());
        return *this;
    }

    string toString() const
    {
        string s;
        s.append("--------------------------- sulink mqtt info begin --------------------------");
        s.append("\nclientId: " + mClientId);
        s.append("\nserverIp: " + mServerIp);
        s.append("\nserverPort: " + to_string(mServerPort));
        s.append("\nuserName: " + mUserName);
        s.append("\npassWord: " + mPassword);
        s.append("\nkeepAlive: " + to_string(mKeepAlive));
        s.append("\npubTopics:");
        for (int i = 0; i < mPubTopics.size(); i++)
        {
            s.append("\n  " + mSubTopics[i]);
        }
        s.append("\nsubTopics:");
        for (int i = 0; i < mSubTopics.size(); i++)
        {
            s.append("\n  " + mSubTopics[i]);
        }
        s.append("\n--------------------------- sulink mqtt info end --------------------------");
        return s;
    }
};

#endif  //  SDK_MQTT_MQTT_CLIENT_MQTT_CLIENT_PARAM_HPP_