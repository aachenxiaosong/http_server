#include "SulinkLiftInitData.hpp"
#include "uni_log.h"

#define SULINK_LIFT_INFO_DATA_TAG "sulink_lift_into_data"

SulinkLiftInitData SulinkLiftInitData::mData;

SulinkLiftInitData :: SulinkLiftInitData()
{
    ifstream ifs(SULINK_LIFT_INIT_DATA_FILE);
    ostringstream oss;
    oss << ifs.rdbuf();
    string raw_data = oss.str();
    ifs.close();
    mInfo = NULL;
    if (!raw_data.empty())
    {
        SulinkPacker packer;
        mInfo = dynamic_cast<SulinkMessageRecvLiftInfo *>(packer.unpack(raw_data));
        mUpdateSpaceIdIndexMap();
        cout << toString() << endl;
    }
};
SulinkLiftInitData :: ~SulinkLiftInitData()
{
    if (mInfo != NULL)
    {
        delete mInfo;
    }
};

void SulinkLiftInitData :: mUpdateInfo(const SulinkMessageRecvLiftInfo &info)
{
    if (mInfo != NULL)
    {
        delete mInfo;
    }
    mInfo = new SulinkMessageRecvLiftInfo(info);
    SulinkPacker packer;
    ofstream ofs(SULINK_LIFT_INIT_DATA_FILE);
    string* sinfo = packer.pack(info);
    istringstream iss(*sinfo);
    ofs << iss.rdbuf();
    ofs.close();
    delete sinfo;
    mUpdateSpaceIdIndexMap();
}

void SulinkLiftInitData :: mUpdateSpaceIdIndexMap() {
    mSpaceIdIndexMap.clear();
    for (int i = 0; i < mInfo->spaces().size(); i++) {
        mSpaceIdIndexMap[mInfo->spaces()[i].id()] = i;
    }
    LOGT(SULINK_LIFT_INFO_DATA_TAG, "space id index map updated");
}

int SulinkLiftInitData :: mGetSpaceIdIndex(const string& space_id) {
    if (mSpaceIdIndexMap.count(space_id) > 0) {
        return mSpaceIdIndexMap[space_id];
    }
    LOGW(SULINK_LIFT_INFO_DATA_TAG, "space id %s not found", space_id.c_str());
    return -1;
}

string SulinkLiftInitData :: mGetSpaceNoBySpaceId(const string& space_id, const string& space_type) {
    int index;
    string id = space_id;
    string no = "";
    while ((index = mGetSpaceIdIndex(id)) >= 0) {
        if (mInfo->spaces()[index].elevatorSpaceType().compare(space_type) == 0) {
            no = mInfo->spaces()[index].elevatorSpaceNo();
            break;
        } else {
            id = mInfo->spaces()[index].pid();
        }
    }
    LOGT(SULINK_LIFT_INFO_DATA_TAG, "got space no \"%s\" by space id %s", no.c_str(), space_id.c_str());
    return no;
}

string SulinkLiftInitData :: mGetClusterIdBySpaceId(const string& space_id) {
    int index;
    string cluster_id = "";
    if ((index = mGetSpaceIdIndex(space_id)) >= 0) {
        cluster_id = mInfo->spaces()[index].clusterControllerIds();
    }
    LOGT(SULINK_LIFT_INFO_DATA_TAG, "got cluster id \"%s\" by space id %s", cluster_id.c_str(), space_id.c_str());
    return cluster_id;
}

string SulinkLiftInitData :: mGetClusterUrlBySpaceId(const string& space_id) {
    int index;
    string cluster_id = mGetClusterIdBySpaceId(space_id);
    string cluster_url = "";
    if (cluster_id.empty() == false)
    {
        vector<LiftInfoCluster> &clusters = mInfo->brandInfo().clusterControllers();
        for (int i = 0; i < clusters.size(); i++)
        {
            if (clusters[i].controllerId().compare(cluster_id) == 0) {
                cluster_url = clusters[i].controllerIp();
                break;
            }
        }
    }
    LOGT(SULINK_LIFT_INFO_DATA_TAG, "got cluster url \"%s\" by space id %s", cluster_url.c_str(), space_id.c_str());
    return cluster_url;
}

string SulinkLiftInitData :: mGetFloorNoBySpaceId(const string& space_id) {
    int index;
    string floor = "";
    if ((index = mGetSpaceIdIndex(space_id)) >= 0) {
        floor = mInfo->spaces()[index].floorLocation();
    }
    LOGT(SULINK_LIFT_INFO_DATA_TAG, "got floor no \"%s\" by space id %s", floor.c_str(), space_id.c_str());
    return floor;
}

string SulinkLiftInitData ::mGetDeviceFloorNo(const string &device_code)
{
    string floor = "";
    for (int i = 0; i < mInfo->accessDevices().size(); i++) {
        if (mInfo->accessDevices()[i].deviceCode().compare(device_code) == 0) {
            floor = mInfo->accessDevices()[i].curFloor();
            break;
        }
    }
    return floor;
}

string SulinkLiftInitData :: mGetDeviceHallNo(const string &device_code)
{
    string hall = "";
    for (int i = 0; i < mInfo->accessDevices().size(); i++) {
        if (mInfo->accessDevices()[i].deviceCode().compare(device_code) == 0) {
            hall = mInfo->accessDevices()[i].hallNo();
            break;
        }
    }
    return hall;
}

int SulinkLiftInitData :: mGetDeviceWorkMode(const string& device_code)
{
    int work_mode = -1;
    for (int i = 0; i < mInfo->accessDevices().size(); i++) {
        if (mInfo->accessDevices()[i].deviceCode().compare(device_code) == 0) {
            work_mode = mInfo->accessDevices()[i].workMode();
            break;
        }
    }
    return work_mode;
}
    
string SulinkLiftInitData :: mGetDevicePubFloors(const string& device_code)
{
    string pub_floors = "";
    for (int i = 0; i < mInfo->accessDevices().size(); i++) {
        if (mInfo->accessDevices()[i].deviceCode().compare(device_code) == 0) {
            pub_floors = mInfo->accessDevices()[i].publicFloor();
            break;
        }
    }
    return pub_floors;
}

string SulinkLiftInitData :: mGetDeviceLiftId(const string& device_code)
{
    string lift_id = "";
    for (int i = 0; i < mInfo->accessDevices().size(); i++) {
        if (mInfo->accessDevices()[i].deviceCode().compare(device_code) == 0) {
            lift_id = mInfo->accessDevices()[i].liftId();
            break;
        }
    }
    return lift_id;
}

const string SulinkLiftInitData :: mToString()
{
    SulinkPacker packer;
    string ret = "";
    if (mInfo != NULL)
    {
        string *package = packer.pack(*mInfo);
        CJsonObject jpackage(*package);
        ret = jpackage.ToFormattedString();
        delete package;
    }
    return ret;
}

//不直接定义mInfo成静态成员的原因是,读文件的部分需要静态初始化,所以要借助静态的mData成员
void SulinkLiftInitData :: updateInfo(const SulinkMessageRecvLiftInfo &info)
{
    return mData.mUpdateInfo(info);
}

string SulinkLiftInitData ::getHomeNoBySpaceId(const string &space_id)
{
    return mData.mGetSpaceNoBySpaceId(space_id, SPACE_TYPE_HOME);
}

string SulinkLiftInitData ::getUnitNoBySpaceId(const string &space_id)
{
    return mData.mGetSpaceNoBySpaceId(space_id, SPACE_TYPE_UNIT);
}

string SulinkLiftInitData ::getBuildingNoBySpaceId(const string &space_id)
{
    return mData.mGetSpaceNoBySpaceId(space_id, SPACE_TYPE_BUILDING);
}

string SulinkLiftInitData ::getClusterIdBySpaceId(const string &space_id)
{
    return mData.mGetClusterIdBySpaceId(space_id);
}

string SulinkLiftInitData :: getClusterUrlBySpaceId(const string &space_id)
{
    return mData.mGetClusterUrlBySpaceId(space_id);
}

string SulinkLiftInitData :: getFloorNoBySpaceId(const string& space_id) {
    return mData.mGetFloorNoBySpaceId(space_id);
}

string SulinkLiftInitData :: getDeviceFloorNo(const string& device_code)
{
    return mData.mGetDeviceFloorNo(device_code);
}

string SulinkLiftInitData :: getDeviceHallNo(const string& device_code)
{
    return mData.mGetDeviceHallNo(device_code);
}

int SulinkLiftInitData :: getDeviceWorkMode(const string& device_code)
{
    return mData.mGetDeviceWorkMode(device_code);
}

string SulinkLiftInitData :: getDevicePubFloors(const string& device_code)
{
    return mData.mGetDevicePubFloors(device_code);
}

string SulinkLiftInitData :: getDeviceLiftId(const string& device_code)
{
    return mData.mGetDeviceLiftId(device_code);
}

const string SulinkLiftInitData :: toString()
{
    return mData.mToString();
}
