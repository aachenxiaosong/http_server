#ifndef APP_UP_SULINK_MQTT_DATA_SULINK_PASS_DEVICE_HPP_
#define APP_UP_SULINK_MQTT_DATA_SULINK_PASS_DEVICE_HPP_

#include <string>
#include <vector>

using namespace std;

struct SulinkPassDevice {
    string deviceCode;
    vector<string> rules; // ruleCode列表,用来快速索引设备上的规则
};

#endif  //  APP_UP_SULINK_MQTT_DATA_SULINK_PASS_DEVICE_HPP_