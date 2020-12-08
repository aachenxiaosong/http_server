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
    //do not check NULL of mConf, just let it crash
    return mConf->getString(key);
}
bool unisound::Config :: getBool(const string &key)
{
    return mConf->getBool(key);
}
int unisound::Config :: getInt(const string &key)
{
    return mConf->getInt(key);
}
double unisound::Config :: getDouble(const string &key)
{
    return mConf->getDouble(key);
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