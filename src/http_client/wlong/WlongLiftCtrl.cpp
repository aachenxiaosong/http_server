#include "WlongLiftCtrl.hpp"
#include "uni_http.h"
#include "uni_log.h"

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
    CJsonObject ojson;
    char request_url[128] = {0};
    const char *headers[1][2] = {{"Content-Type", "application/json;charset=UTF-8"}};
    const char *request;
    char *result = NULL;
    int ret;
    ojson.Add("appId", appid);
    ojson.Add("appSecret", appsecret);
    ojson.Add("licence", licence);
    ojson.Add("deviceId", lift_id);
    ojson.Add("openFloor", open_floors);
    ojson.Add("unlockFloor", unlock_floors);
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), CALL_ELEVATOR_BY_FLOOR_PATH);
    request = ojson.ToString().c_str();
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
    CJsonObject ojson;
    char request_url[128] = {0};
    const char *headers[1][2] = {{"Content-Type", "application/json;charset=UTF-8"}};
    const char *request;
    char *result = NULL;
    int ret;
    ojson.Add("appId", appid);
    ojson.Add("appSecret", appsecret);
    ojson.Add("licence", licence);
    ojson.Add("deviceId", cluster_id);
    ojson.Add("fromFloor", from_floor);
    ojson.Add("upDown", updown);
    ojson.Add("openFloor", open_floors);
    ojson.Add("openTime", open_time);
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), BOOKING_ELEVATOR_PATH);
    request = ojson.ToString().c_str();
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
