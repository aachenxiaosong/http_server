#include "SulinkLiftInitData.hpp"

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

const string SulinkLiftInitData :: toString()
{
    return mData.mToString();
}
