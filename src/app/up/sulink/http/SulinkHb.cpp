#include "SulinkHb.hpp"
#include "SulinkSignature.hpp"
#include "HttpClient.hpp"
#include "configurable_info.h"
#include "UniUtil.hpp"
#include "CJsonObject.hpp"
#include "uni_log.h"

#define SULINK_HB_TAG "sulink_hb_http"
using namespace neb;


int SulinkHb :: request() {
    map<string, string> headers;
    map<string, string> params;
    long timestamp = UniUtil::timestampMs();
    params["deviceCode"] = UniUtil::deviceCode();
    headers["Content-Type"] = "application/json";
    headers["brand"] = SULINK_BRAND;
    headers["timestamp"] = to_string(timestamp);
    headers["signature"] = SulinkSignature::build(params, to_string(timestamp));
    CJsonObject jcontent;
    jcontent.Add("deviceCode", UniUtil::deviceCode());
    jcontent.Add("upTime", timestamp);
    jcontent.Add("cpu", UniUtil::cpuUtility());
    jcontent.Add("memory", UniUtil::memoryUtility());
    jcontent.Add("versionNumber", MY_VERSION);
    string content = jcontent.ToString();
    string result;
    int rc = HttpClient::post(SULINK_HB_URL, content, result, headers);
    if (rc != 0 || result.empty()) {
        LOGE(SULINK_HB_TAG, "request failed");
    } else {
        int ret_code = -1;
        CJsonObject jresult(result);
        jresult.Get("retcode", ret_code);
        LOGT(SULINK_HB_TAG, "request ok, retcode is %d", ret_code);
        rc = ret_code;
    }
    return rc;
}
