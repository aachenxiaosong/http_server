#ifndef APP_UP_SULINK_MQTT_DATA_SULINK_LIFT_INIT_HPP_
#define APP_UP_SULINK_MQTT_DATA_SULINK_LIFT_INIT_HPP_

#include "SulinkMessage.hpp"
#include "SulinkPacker.hpp"
#include "Mq.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

//TODO: 数据共享需要解耦一下
class SulinkLiftInitData {
private:
    static SulinkLiftInitData mData;
    SulinkMessageRecvLiftInfo *mInfo;
    map<string, int> mSpaceIdIndexMap;
    Mq mMq;
    SulinkLiftInitData();
    ~SulinkLiftInitData();
    int mLoadFromConfig();
    void mUpdateInfo(const SulinkMessageRecvLiftInfo& info);
    void mUpdateSpaceIdIndexMap();
    string mGetAppId();
    string mGetAppSecret();
    string mGetLicense();
    string mGetBrandCode();
    int mGetSpaceIdIndex(const string& space_id);
    string mGetSpaceNoBySpaceId(const string& space_id, const string& space_type);
    string mGetClusterIdBySpaceId(const string& space_id);
    string mGetClusterUrlBySpaceId(const string& space_id);
    string mGetFloorNoBySpaceId(const string& space_id);
    string mGetDeviceFloorNo(const string& device_code);
    string mGetDeviceHallNo(const string& device_code);
    int mGetDeviceWorkMode(const string& device_code);
    string mGetDevicePubFloors(const string& device_code);
    string mGetDeviceLiftId(const string& device_code);
    const string mToString();
public:
    //不直接定义mInfo成静态成员的原因是,读文件的部分需要静态初始化,所以要借助静态的mData成员
    static int loadFromConfig();
    static bool inited();
    static void updateInfo(const SulinkMessageRecvLiftInfo& info);
    static string getAppId();
    static string getAppSecret();
    static string getLicense();
    static string getBrandCode();
    static string getHomeNoBySpaceId(const string& space_id);
    static string getUnitNoBySpaceId(const string& space_id);
    static string getBuildingNoBySpaceId(const string& space_id);
    static string getClusterIdBySpaceId(const string& space_id);
    static string getClusterUrlBySpaceId(const string& space_id);
    static string getFloorNoBySpaceId(const string& space_id);
    static string getDeviceFloorNo(const string& device_code);
    static string getDeviceHallNo(const string& device_code);
    static int getDeviceWorkMode(const string& device_code);
    static string getDevicePubFloors(const string& device_code);
    static string getDeviceLiftId(const string& device_code);
    static const string toString();
};

#endif  //  APP_UP_SULINK_MQTT_DATA_SULINK_LIFT_INIT_HPP_
