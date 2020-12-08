#include "SulinkDeviceRegister.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkSignature.hpp"
#include "HttpClient.hpp"
#include "configurable_info.h"
#include "UniDeviceInfo.hpp"
#include "SulinkConfigData.hpp"
#include "UniUtil.hpp"
#include "CJsonObject.hpp"
#include "UniLog.hpp"

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
    jdata.Get("clientId", mParam.clientId);
    jconnection.Get("ip", mParam.serverIp);
    jconnection.Get("port", mParam.serverPort);
    jconnection.Get("username", mParam.userName);
    jconnection.Get("password", mParam.password);
    jconnection.Get("keepAlive", mParam.keepAlive);
    mParam.pubTopics.clear();
    for (int i = 0; i < jpub.GetArraySize(); i++) {
        jpub.Get(i, svalue);
        mParam.pubTopics.push_back(svalue);
    }
    mParam.subTopics.clear();
    for (int i = 0; i < jsub.GetArraySize(); i++) {
        jsub.Get(i, svalue);
        mParam.subTopics.push_back(svalue);
    }
    return 0;
}

int SulinkDeviceRegister :: request()
{
    map<string, string> headers;
    map<string, string> params;
    long timestamp = unisound::UniUtil::timestampMs();
    params["deviceCode"] = unisound::UniDeviceInfo::getUdid();
    headers["Content-Type"] = "application/json";
    headers["brand"] = SulinkConfigData::getBrand();
    headers["timestamp"] = to_string(timestamp);
    headers["signature"] = SulinkSignature::build(params, to_string(timestamp));
    string content = "{\"deviceCode\":\"" + unisound::UniDeviceInfo::getUdid() + "\"}";
    string result;
    int rc = HttpClient::post(SulinkConfigData::getUrlDeviceRegister(), content, result, headers);
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
