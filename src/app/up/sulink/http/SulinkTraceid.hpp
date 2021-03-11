#ifndef APP_UP_SULINK_HTTP_SULINK_TRACEID_HPP_
#define APP_UP_SULINK_HTTP_SULINK_TRACEID_HPP_

#include "configurable_info.h"
#include "UniDeviceInfo.hpp"
#include "UniUtil.hpp"
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class SulinkTraceid {
public:
    static string build(const string& timestamp) {
        string trace_id;
        string device_code = unisound::UniDeviceInfo::getUdid();
        string uuid = unisound::UniUtil::uuid();
        string trace_id_str = timestamp + device_code + uuid.substr(uuid.length() - 7, uuid.length());
        string trace_id_md5 =  unisound::UniUtil::md5(trace_id_str);
        transform(trace_id_md5.begin(), trace_id_md5.end(), trace_id_md5.begin(), ::toupper);
        trace_id = trace_id_md5;
        return trace_id;
    }
};

#endif  //  APP_UP_SULINK_HTTP_SULINK_TRACEID_HPP_

