#ifndef  SDK_MQTT_MQTT_HANDLE_IMQTT_PACKER_HPP_
#define  SDK_MQTT_MQTT_HANDLE_IMQTT_PACKER_HPP_

#include <string>
#include <vector>
#include "ISulinkMessage.hpp"

using namespace std;


/* data<-->数据结构 */
class ISulinkPacker {
private:
    string mName;
public:
    ISulinkPacker(const char *name) {mName = name;}
    string getName() {return mName;}
    virtual ~ISulinkPacker() {};
    //return message need to be released by app
    virtual ISulinkMessage* unpack(const string& raw_data) = 0;
    //return string need to be released by app
    virtual string* pack(const ISulinkMessage &message) = 0;
};

#endif  //  SDK_MQTT_MQTT_HANDLE_IMQTT_PACKER_HPP_