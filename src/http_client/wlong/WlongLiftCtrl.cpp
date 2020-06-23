#include "WlongLiftCtrl.hpp"
#include "uni_http.h"
#include "uni_log.h"
#include "configurable_info.h"

#define WLONG_3P_CTRL "wlong_3p_ctrl"

WlongLiftCtrl :: WlongLiftCtrl(const string& url, const string& appid, const string& appsecret, const string& licence) {
    this->url = url;
    this->appid = appid;
    this->appsecret = appsecret;
    this->licence = licence;
    LOGT(WLONG_3P_CTRL, "WlongLiftCtrl created");
}

int WlongLiftCtrl :: _parse_result(char *result, WlongResponse& response) {
    CJsonObject ojson(result);
    int ivalue;
    string svalue;
    LOGT(WLONG_3P_CTRL, "parsing result: %s", result);
    if (true != ojson.Get("code", ivalue)) {
        LOGT(WLONG_3P_CTRL, "parse code failed for call_elevator_by_floor: %s", result);
        return -1;
    }
    response.code = ivalue;
    if (true != ojson.Get("message", svalue)) {
        LOGT(WLONG_3P_CTRL, "parse message failed for call_elevator_by_floor: %s", result);
        return -1;
    }
    response.msg = svalue;
    if (true != ojson.Get("data", svalue)) {
        LOGT(WLONG_3P_CTRL, "parse data failed for call_elevator_by_floor: %s", result);
        response.data = "";
        //return -1;
    }
    response.data = svalue;
    return 0;
}

int WlongLiftCtrl :: callElevatorByFoor(int lift_id, string& open_floors, string& unlock_floors, WlongResponse& response) {
    char request_url[128] = {0};
    char request[1024] = {0};
    const char *headers[1][2] = {{"Content-Type", "application/x-www-form-urlencoded;charset=UTF-8"}};
    char *result = NULL;
    int ret;
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), CALL_ELEVATOR_BY_FLOOR_PATH);
    snprintf(request, sizeof(request), "appId=%s&appSecret=%s&licence=%s&deviceId=%d&openFloor=%s&unlockFloor=%s",
             appid.c_str(), appsecret.c_str(), licence.c_str(), lift_id, open_floors.c_str(), unlock_floors.c_str());
    LOGT(WLONG_3P_CTRL, "call_elevator_by_floor url:%s, request:%s", request_url, request);
    #if !STUB_ENABLE
    ret = HttpPostWithHeadersTimeout(request_url, request, headers, 1, 5, &result);
    #else
    ret = 0;
    result = (char *)uni_malloc(1024);
    sprintf(result, "%s", "{\"code\": 0, \"message\": \"OK\", \"data\":\"no data\"}");
    #endif
    if (0 == ret &&NULL != result &&
        0 == _parse_result(result, response)) {
        LOGT(WLONG_3P_CTRL, "wlong call_elevator_by_floor calling succeeded");
        ret = 0;
    } else {
        LOGE(WLONG_3P_CTRL, "wlong call_elevator_by_floor calling failed");
        ret = -1;
    }
    if (NULL != result) {
        uni_free(result);
    }
    return ret;
}

int WlongLiftCtrl :: bookingElevator(int cluster_id, string& from_floor, string& updown, string& open_floors, int open_time, WlongResponse& response) {
    char request_url[128] = {0};
    char request[1024] = {0};
    const char *headers[1][2] = {{"Content-Type", "application/x-www-form-urlencoded;charset=UTF-8"}};
    char *result = NULL;
    int ret;
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), BOOKING_ELEVATOR_PATH);
    snprintf(request, sizeof(request), 
             "appId=%s&appSecret=%s&licence=%s&deviceId=%d&fromFloor=%s&upDown=%s&openFloor=%s&openTime=%d",
             appid.c_str(), appsecret.c_str(), licence.c_str(), cluster_id, from_floor.c_str(), updown.c_str(), open_floors.c_str(), open_time);
    LOGT(WLONG_3P_CTRL, "booking_elevator url:%s, request:%s", request_url, request);
    #if !STUB_ENABLE
    ret = HttpPostWithHeadersTimeout(request_url, request, headers, 1, 5, &result);
    #else
    ret = 0;
    result = (char *)uni_malloc(1024);
    sprintf(result, "%s", "{\"code\": 0, \"message\": \"OK\", \"data\":\"no data\"}");
    #endif
    if (0 == ret &&NULL != result &&
        0 == _parse_result(result, response)) {
        LOGT(WLONG_3P_CTRL, "wlong booking_elevator calling succeeded");
        ret = 0;
    } else {
        LOGE(WLONG_3P_CTRL, "wlong booking_elevator calling failed");
        ret = -1;
    }
    if (NULL != result) {
        uni_free(result);
    }
    return ret;
}
