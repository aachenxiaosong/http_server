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
    string mPort;
    string mUserName;
    string mPassword;
    vector<string> mSubTopics;
    vector<string> mPubTopics;
public:
    SulinkDeviceRegister() = default;
    ~SulinkDeviceRegister() {
        mSubTopics.clear();
        mPubTopics.clear();
    }
    const string& getClientId() {return mClientId;}
    const string& getServerIp() {return mServerIp;}
    const string& getPort() {return mPort;}
    const string& getUserName() {return mUserName;}
    const string& getPassword() {return mPassword;}
    const vector<string>& getSubTopics() {return mSubTopics;}
    const vector<string>& getPubTopics() {return mPubTopics;}
public:
    int request();
};

#endif  //  APP_UP_SULINK_HTTP_SULINK_DEVICE_REGISTER_HPP_
