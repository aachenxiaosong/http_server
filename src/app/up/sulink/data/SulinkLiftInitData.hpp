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
    SulinkLiftInitData() {
        ifstream ifs(SULINK_LIFT_INIT_DATA_FILE);
        ostringstream oss;
        oss << ifs.rdbuf();
        string raw_data = oss.str();
        ifs.close();
        mInfo = NULL;;
        if (!raw_data.empty()) {
            SulinkPacker packer;
            mInfo = dynamic_cast<SulinkMessageRecvLiftInfo*>(packer.unpack(raw_data));
        }
    };
    ~SulinkLiftInitData() {
        if (mInfo != NULL) {
            delete mInfo;
        }
    };

    void mUpdateInfo(const SulinkMessageRecvLiftInfo& info) {
        if (mInfo != NULL) {
            delete mInfo;
        }
        mInfo = new SulinkMessageRecvLiftInfo(info);
    }

    const string mToString() {
        SulinkPacker packer;
        string ret = "";
        if (mInfo != NULL) {
            string *package = packer.pack(*mInfo);
            ret = *package;
            delete package;
        }
        return ret;
    }
public:
    //不直接定义mInfo成静态成员的原因是,读文件的部分需要静态初始化,所以要借助静态的mData成员
    static void updateInfo(const SulinkMessageRecvLiftInfo& info) {
        return mData.updateInfo(info);
    }
    static const string toString() {
        return mData.mToString();
    }
};

//SulinkLiftInitData SulinkLiftInitData::mData;


#endif  //  APP_UP_SULINK_MQTT_DATA_SULINK_LIFT_INIT_HPP_
