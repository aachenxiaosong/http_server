#include "SulinkDeviceRegister.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkSignature.hpp"
#include "HttpClient.hpp"
#include "configurable_info.h"
#include "UniUtil.hpp"
#include "CJsonObject.hpp"
#include "uni_log.h"

#define SULINK_DEVICE_REG_TAG "sulink_device_register_http"

int SulinkDeviceRegister ::request()
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
        LOGT(SULINK_DEVICE_REG_TAG, "request failed");
    } else {
        LOGT(SULINK_DEVICE_REG_TAG, "request ok");
    }
    //TODO 解析结果
    return rc;
}
