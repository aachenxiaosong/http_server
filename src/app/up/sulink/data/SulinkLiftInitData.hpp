#ifndef APP_UP_SULINK_MQTT_DATA_SULINK_LIFT_INIT_HPP_
#define APP_UP_SULINK_MQTT_DATA_SULINK_LIFT_INIT_HPP_

#include "SulinkMessage.hpp"
#include "SulinkPacker.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define SULINK_LIFT_INIT_DATA_FILE "data/lift_init_info.txt"

class SulinkLiftInitData {
private:
    static SulinkLiftInitData mData;
    SulinkMessageRecvLiftInfo *mInfo;
    SulinkLiftInitData();
    ~SulinkLiftInitData();
    void mUpdateInfo(const SulinkMessageRecvLiftInfo& info);
    const string mToString();
public:
    //不直接定义mInfo成静态成员的原因是,读文件的部分需要静态初始化,所以要借助静态的mData成员
    static void updateInfo(const SulinkMessageRecvLiftInfo& info);
    static const string toString();
};

#endif  //  APP_UP_SULINK_MQTT_DATA_SULINK_LIFT_INIT_HPP_
