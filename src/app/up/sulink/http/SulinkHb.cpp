#include "SulinkHb.hpp"
#include "SulinkSignature.hpp"
#include "HttpClient.hpp"
#include "configurable_info.h"
#include "UniDeviceInfo.hpp"
#include "UniUtil.hpp"
#include "CJsonObject.hpp"
#include "UniLog.hpp"

#define SULINK_HB_TAG "sulink_hb_http"
using namespace neb;

SulinkHb :: SulinkHb() {
    mIsRunning = false;
}

int SulinkHb :: startHbTask() {
    mHbThread = new thread(hbTask, this);
    if (mHbThread == NULL) {
        LOGE(SULINK_HB_TAG, "start hb task failed");
        return -1;
    }
    return 0;
}

SulinkHb :: ~SulinkHb() {
    mIsRunning = false;
    if (mHbThread) {
        mHbThread->join();
        delete mHbThread;
    }
}

int SulinkHb :: request() {
    map<string, string> headers;
    map<string, string> params;
    string device_code = unisound::UniDeviceInfo::getUdid();
    double cpu_utility = unisound::UniDeviceInfo::getCpuUtility();
    double mem_utility = unisound::UniDeviceInfo::getMemUtility();
    long timestamp = UniUtil::timestampMs();
    params["deviceCode"] = device_code;
    params["upTime"] = to_string(timestamp);
    params["cpu"] = to_string(cpu_utility);
    params["memory"] = to_string(mem_utility);
    params["versionNumber"] = MY_VERSION;
    headers["Content-Type"] = "application/json";
    headers["brand"] = SULINK_BRAND;
    headers["timestamp"] = to_string(timestamp);
    headers["signature"] = SulinkSignature::build(params, to_string(timestamp));
    CJsonObject jcontent;
    jcontent.Add("deviceCode", device_code);
    jcontent.Add("upTime", timestamp);
    jcontent.Add("cpu", to_string(cpu_utility));
    jcontent.Add("memory", to_string(mem_utility));
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

void SulinkHb :: hbTask(void *arg)
{
    SulinkHb* hb = (SulinkHb*)arg;
    hb->mIsRunning = true;
    while (hb->mIsRunning) {
        hb->request();
        sleep(HB_INTERVAL);
    }
}
