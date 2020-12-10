#include "UniConfig.hpp"
#include "UniLog.hpp"

#define CONFIG_TAG "config"

unisound::Config :: Config(const string &path)
{
    mConf = new Poco::Util::IniFileConfiguration(path);
    if (mConf != NULL)
    {
        mPath = path;
        LOGT(CONFIG_TAG, "config file " + mPath + " loaded OK");
    }
    LOGE(CONFIG_TAG, "config file " + mPath + " loaded failed");
}

string unisound::Config :: getString(const string &key)
{
    string ret;
    try {
        ret = mConf->getString(key);
    } catch (Poco::NotFoundException& e) {
        LOGW(CONFIG_TAG, "config of %s(string) not found", key.c_str());
    }
    return ret;
}
bool unisound::Config ::getBool(const string &key)
{
    bool ret = 0;
    try
    {
        ret = mConf->getBool(key);
    }
    catch (Poco::NotFoundException &e)
    {
        LOGW(CONFIG_TAG, "config of %s(bool) not found", key.c_str());
    }
    return ret;
}

int unisound::Config :: getInt(const string &key)
{
    int ret = 0;
    try
    {
        ret = mConf->getInt(key);
    }
    catch (Poco::NotFoundException &e)
    {
        LOGW(CONFIG_TAG, "config of %s(int) not found", key.c_str());
    }
    return ret;
}

double unisound::Config :: getDouble(const string &key)
{
    double ret = 0.0;
    try
    {
        ret = mConf->getDouble(key);
    }
    catch (Poco::NotFoundException &e)
    {
        LOGW(CONFIG_TAG, "config of %s(double) not found", key.c_str());
    }
    return ret;
}

unisound::Config unisound::UniConfig :: mConfig("config/app_conf.properties");

string unisound::UniConfig :: getString(const string &key)
{
    return mConfig.getString(key);
}
bool unisound::UniConfig :: getBool(const string &key)
{
    return mConfig.getBool(key);
}
int unisound::UniConfig :: getInt(const string &key)
{
    return mConfig.getInt(key);
}
double unisound::UniConfig :: getDouble(const string &key)
{
    return mConfig.getDouble(key);
}