#include "SulinkTimeSync.hpp"
#include "SulinkSignature.hpp"
#include "HttpClient.hpp"
#include "configurable_info.h"
#include "UniUtil.hpp"
#include "CJsonObject.hpp"
#include "uni_log.h"

#define SULINK_TIME_SYNC_TAG "sulink_time_sync_http"
using namespace neb;

int SulinkTimeSync :: calculateLocalTime() {
    long local_time;
    if (T4 - T1 > 10 * 1000) { //请求超过10s,认为本次同步无效
        LOGE(SULINK_TIME_SYNC_TAG, "invalid time sync for taking too much time T4=%ld T1=%ld", T4, T1);
        return -1;
    }
    local_time = (T2 + T3 + T4 - T1) / 2;
    LOGT(SULINK_TIME_SYNC_TAG, "new local timestamp=%ld, T4=%ld", local_time, T4);
    return 0;
}

int SulinkTimeSync :: parseResult(const string& result) {
    int ret_code = -1;
    CJsonObject jresult(result);
    if (jresult.Get("retcode", ret_code) != true || ret_code != 0) {
        LOGE(SULINK_TIME_SYNC_TAG, "request failed, retcode is %d", ret_code);
        return -1;
    }
    CJsonObject jdata;
    if (jresult.Get("data", jdata) != true) {
        LOGE(SULINK_TIME_SYNC_TAG, "parse data failed");
        return -1;
    }
    if (jdata.Get("cloudRequestTime", T2) != true || jdata.Get("cloudResponseTime", T3) != true) {
        LOGE(SULINK_TIME_SYNC_TAG, "parse T2 or T3 failed");
        return -1;
    }
    T4 = UniUtil::timestampMs();
    return calculateLocalTime();
}

int SulinkTimeSync :: request() {
    map<string, string> headers;
    map<string, string> params;
    long timestamp = UniUtil::timestampMs();
    T1 = timestamp;
    params["localTime"] = to_string(timestamp);
    headers["Content-Type"] = "application/json";
    headers["brand"] = SULINK_BRAND;
    headers["timestamp"] = timestamp;
    headers["signature"] = SulinkSignature::build(params, to_string(timestamp));
    CJsonObject jcontent;
    jcontent.Add("localTime", timestamp);
    string content = jcontent.ToString();
    string result;
    int rc = HttpClient::post(SULINK_TIME_SYNC_URL, content, result, headers);
    if (rc != 0 || result.empty()) {
        LOGE(SULINK_TIME_SYNC_TAG, "request failed");
    } else {
        rc = parseResult(result);
    }
    return rc;
}
