#include "SulinkDeviceRegister.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkSignature.hpp"
#include "HttpClient.hpp"
#include "configurable_info.h"
#include "UniUtil.hpp"
#include "CJsonObject.hpp"
#include "uni_log.h"

using namespace neb;
#define SULINK_DEVICE_REG_TAG "sulink_device_register_http"

int SulinkDeviceRegister :: parseResult(const string& result) {
    CJsonObject jresult(result);
    CJsonObject jdata;
    CJsonObject jconnection;
    CJsonObject jtopics;
    CJsonObject jpub;
    CJsonObject jsub;
    int retcode;
    CJsonObject jvalue;
    int ivalue;
    string svalue;
    if (jresult.Get("retcode", retcode) != true) {
        LOGT(SULINK_DEVICE_REG_TAG, "parse retcode failed");
        return -1;
    }
    if (retcode != 0) {
        LOGT(SULINK_DEVICE_REG_TAG, "retcode(%d) is not 0", retcode);
        return -1;
    }
    if (jresult.Get("data", jdata) != true) {
        LOGT(SULINK_DEVICE_REG_TAG, "parse data failed");
        return -1;
    }
    if (jdata.Get("connection", jconnection) != true) {
        LOGT(SULINK_DEVICE_REG_TAG, "parse connection failed");
        return -1;
    }
    if (jdata.Get("topics", jtopics) != true) {
        LOGT(SULINK_DEVICE_REG_TAG, "parse topics failed");
        return -1;
    }
    if (jtopics.Get("pub", jpub) != true) {
        LOGT(SULINK_DEVICE_REG_TAG, "parse pub failed");
        return -1;
    }
    if (jtopics.Get("sub", jsub) != true) {
        LOGT(SULINK_DEVICE_REG_TAG, "parse sub failed");
        return -1;
    }
    jdata.Get("clientId", mClientId);
    jconnection.Get("ip", mServerIp);
    jconnection.Get("port", mServerPort);
    jconnection.Get("username", mUserName);
    jconnection.Get("password", mPassword);
    jconnection.Get("keepAlive", mKeepAlive);
    mPubTopics.clear();
    for (int i = 0; i < jpub.GetArraySize(); i++) {
        jpub.Get(i, svalue);
        mPubTopics.push_back(svalue);
    }
    mSubTopics.clear();
    for (int i = 0; i < jsub.GetArraySize(); i++) {
        jsub.Get(i, svalue);
        mSubTopics.push_back(svalue);
    }
    return 0;
}

int SulinkDeviceRegister :: request()
{
    map<string, string> headers;
    map<string, string> params;
    string timestamp = UniUtil::timestampMs();
    params["deviceCode"] = UniUtil::deviceCode();
    headers["Content-Type"] = "application/json";
    headers["brand"] = SULINK_BRAND;
    headers["timestamp"] = timestamp;
    headers["signature"] = SulinkSignature::build(params, timestamp);
    string content = "{\"deviceCode\":\"" + UniUtil::deviceCode() + "\"}";
    string result;
    int rc = HttpClient::post(SULINK_DEVICE_REGISTER_URL, content, result, headers);
    if (rc != 0) {
        LOGE(SULINK_DEVICE_REG_TAG, "request failed");
    } else {
        LOGT(SULINK_DEVICE_REG_TAG, "request ok");
        if (!result.empty()) {
            rc = parseResult(result);
        }
    }
    return rc;
}

string SulinkDeviceRegister :: toString() const {
    string s;
    s.append("--------------------------- sulink mqtt info begin --------------------------");
    s.append("\nclientId: " + mClientId);
    s.append("\nserverIp: " + mServerIp);
    s.append("\nserverPort: " + to_string(mServerPort));
    s.append("\nuserName: " + mUserName);
    s.append("\npassWord: " + mPassword);
    s.append("\nkeepAlive: " + to_string(mKeepAlive));
    s.append("\npubTopics:");
    for (int i = 0; i < mPubTopics.size(); i++) {
        s.append("\n  " + mSubTopics[i]);
    }
    s.append("\nsubTopics:");
    for (int i = 0; i < mSubTopics.size(); i++) {
        s.append("\n  " + mSubTopics[i]);
    }
    s.append("\n--------------------------- sulink mqtt info end --------------------------");
    return s;
}
