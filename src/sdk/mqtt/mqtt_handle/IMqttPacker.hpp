#ifndef  SDK_MQTT_MQTT_HANDLE_IMQTT_PACKER_HPP_
#define  SDK_MQTT_MQTT_HANDLE_IMQTT_PACKER_HPP_

#include <string>
#include <vector>
#include "IMqttMessage.hpp"

using namespace std;


/* data<-->数据结构 */
class IMqttPacker {
private:
    string mName;
public:
    IMqttPacker(const char *name) {mName = name;}
    string getName() {return mName;}
    virtual ~IMqttPacker();
    //return message need to be released by app
    virtual IMqttMessage* unpack(const string& raw_data) = 0;
    //return string need to be released by app
    virtual string* pack(const IMqttMessage &message) = 0;
};

#endif  //  SDK_MQTT_MQTT_HANDLE_IMQTT_PACKER_HPP_