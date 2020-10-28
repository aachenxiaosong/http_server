#ifndef APP_UP_SULINK_HTTP_SULINK_DEVICE_REGISTER_HPP_
#define APP_UP_SULINK_HTTP_SULINK_DEVICE_REGISTER_HPP_

#include "uni_device.h"
#include <string>
#include <vector>

using namespace std;

class SulinkDeviceRegister {
private:
    string mClientId;
    string mServerIp;
    int mServerPort;
    string mUserName;
    string mPassword;
    int mKeepAlive;
    vector<string> mSubTopics;
    vector<string> mPubTopics;
public:
    SulinkDeviceRegister() {
        mServerPort = 0;
        mKeepAlive = 0;
    }
    ~SulinkDeviceRegister() {
        mSubTopics.clear();
        mPubTopics.clear();
    }
    const string& getClientId() {return mClientId;}
    const string& getServerIp() {return mServerIp;}
    const int getServerPort() {return mServerPort;}
    const string& getUserName() {return mUserName;}
    const string& getPassword() {return mPassword;}
    const vector<string>& getSubTopics() {return mSubTopics;}
    const vector<string>& getPubTopics() {return mPubTopics;}
private:
    int parseResult(const string& result);
public:
    int request();
    string toString() const;
};

#endif  //  APP_UP_SULINK_HTTP_SULINK_DEVICE_REGISTER_HPP_
