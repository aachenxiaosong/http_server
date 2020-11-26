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
    CJsonObject jvalue;
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
        if (true == ojson.Get("data", jvalue)) {
            response.data = jvalue.ToString();
        } else {
            LOGT(WLONG_3P_CTRL, "parse data failed for call_elevator_by_floor: %s", result);
            response.data = "";
            //return -1;
        }
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

int WlongLiftCtrl :: bookElevator(int cluster_id, string& from_floor, string& updown, string& unlock_floors, int unlock_time, WlongResponse& response) {
    char request_url[128] = {0};
    char request[1024] = {0};
    const char *headers[1][2] = {{"Content-Type", "application/x-www-form-urlencoded;charset=UTF-8"}};
    char *result = NULL;
    int ret;
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), BOOKING_ELEVATOR_PATH);
    snprintf(request, sizeof(request), 
             "appId=%s&appSecret=%s&licence=%s&deviceId=%d&fromFloor=%s&upDown=%s&openFloor=%s&openTime=%d",
             appid.c_str(), appsecret.c_str(), licence.c_str(), cluster_id, from_floor.c_str(), updown.c_str(), unlock_floors.c_str(), unlock_time);
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
        LOGE(WLONG_3P_CTRL, "wlong booking_elevator calling failed, ret = %d", ret);
        ret = -1;
    }
    if (NULL != result) {
        uni_free(result);
    }
    return ret;
}

int WlongLiftCtrl :: reserveElevator(int cluster_id, string& from_floor, string& open_floor, WlongResponse& response) {
    char request_url[128] = {0};
    char request[1024] = {0};
    const char *headers[1][2] = {{"Content-Type", "application/x-www-form-urlencoded;charset=UTF-8"}};
    char *result = NULL;
    int ret;
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), RESERVE_ELEVATOR_PATH);
    snprintf(request, sizeof(request), 
             "appId=%s&appSecret=%s&licence=%s&deviceId=%d&fromFloor=%s&toFloor=%s&vipTime=%d",
             appid.c_str(), appsecret.c_str(), licence.c_str(), cluster_id, from_floor.c_str(), open_floor.c_str(), 0);
    LOGT(WLONG_3P_CTRL, "reserve_elevator url:%s, request:%s", request_url, request);
    #if !STUB_ENABLE
    ret = HttpPostWithHeadersTimeout(request_url, request, headers, 1, 5, &result);
    #else
    ret = 0;
    result = (char *)uni_malloc(1024);
    sprintf(result, "%s", "{\"code\": 0, \"message\": \"OK\", \"data\":\"no data\"}");
    #endif
    if (0 == ret &&NULL != result &&
        0 == _parse_result(result, response)) {
        LOGT(WLONG_3P_CTRL, "wlong reserve_elevator calling succeeded");
        ret = 0;
    } else {
        LOGE(WLONG_3P_CTRL, "wlong reserve_elevator calling failed, ret = %d", ret);
        ret = -1;
    }
    if (NULL != result) {
        uni_free(result);
    }
    return ret;
}


int WlongLiftCtrl :: aliveTest(string &message) {
    char request_url[128] = {0};
    char request[1024] = {0};
    WlongResponse response;
    const char *headers[1][2] = {{"Content-Type", "application/x-www-form-urlencoded;charset=UTF-8"}};
    char *result = NULL;
    int ret;
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), CONTROLLER_INFO_PATH);
    snprintf(request, sizeof(request), 
             "appId=%s&appSecret=%s&licence=%s&deviceId=1",
             appid.c_str(), appsecret.c_str(), licence.c_str());
    LOGT(WLONG_3P_CTRL, "alive test url:%s, request:%s", request_url, request);
    #if !STUB_ENABLE
    ret = HttpPostWithHeadersTimeout(request_url, request, headers, 1, 5, &result);
    #else
    ret = 0;
    result = (char *)uni_malloc(1024);
    sprintf(result, "%s", "{\"code\": 0, \"message\": \"OK\", \"data\":\"no data\"}");
    #endif
    if (0 == ret &&NULL != result &&
        0 == _parse_result(result, response)) {
        LOGT(WLONG_3P_CTRL, "wlong alive_test calling succeeded");
        if (response.code == 0) {
            ret = 0;
        } else {
            ret = -1;
        }
        message = response.msg;
    } else {
        LOGE(WLONG_3P_CTRL, "wlong alive test calling failed, ret = %d", ret);
        message = "calling wlong api failed";
        ret = -1;
    }
    if (NULL != result) {
        uni_free(result);
    }
    return ret;
}

static int _h2d(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return 0;
}

int WlongLiftCtrl :: _parse_lift_status(char *result, WlongLiftStatus& response) {
    CJsonObject ojson(result);
    int ivalue;
    string svalue;
    CJsonObject jvalue;
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
    if (true != ojson.Get("data", svalue))
    {

        if (true != ojson.Get("data", jvalue))
        {
            LOGT(WLONG_3P_CTRL, "parse data failed for call_elevator_by_floor: %s", result);
            return -1;
        }
        if (true != jvalue.Get("status", svalue) || svalue.length() < 8)
        {
            LOGT(WLONG_3P_CTRL, "parse status failed for call_elevator_by_floor: %s", result);
            return -1;
        }
        string status = svalue;

        int cur_floor = _h2d(status[2]) * 16 + _h2d(status[3]);
        if (status[1] == '1')
        {
            cur_floor *= -1;
        }
        response.cur_floor = cur_floor;
        if (status[5] == '0')
        {
            response.stopped = 1;
            response.upward = 0;
            response.status_error = 0;
        }
        else if (status[5] == '1')
        {
            response.stopped = 0;
            response.upward = 0;
            response.status_error = 0;
        }
        else if (status[5] == '2')
        {
            response.stopped = 0;
            response.upward = 1;
            response.status_error = 0;
        }
        else
        {
            response.stopped = 0;
            response.upward = 0;
            response.status_error = 1;
        }
        if (status[7] == '0')
        {
            response.open = 0;
            response.closed = 1;
            response.opening = 0;
            response.closing = 0;
            response.door_error = 0;
        }
        else if (status[7] == '1')
        {
            response.open = 0;
            response.closed = 0;
            response.opening = 1;
            response.closing = 0;
            response.door_error = 0;
        }
        else if (status[7] == '2')
        {
            response.open = 0;
            response.closed = 0;
            response.opening = 0;
            response.closing = 1;
            response.door_error = 0;
        }
        else if (status[7] == '3')
        {
            response.open = 0;
            response.closed = 0;
            response.opening = 0;
            response.closing = 0;
            response.door_error = 0;
        }
        else if (status[7] == '4')
        {
            response.open = 1;
            response.closed = 0;
            response.opening = 0;
            response.closing = 0;
            response.door_error = 0;
        }
        else
        {
            response.open = 0;
            response.closed = 0;
            response.opening = 0;
            response.closing = 0;
            response.door_error = 1;
        }
    }
    return 0;
}

int WlongLiftCtrl :: getElevatorStatus(int elevator_id, WlongLiftStatus& response) {
    char request_url[128] = {0};
    char request[1024] = {0};
    const char *headers[1][2] = {{"Content-Type", "application/x-www-form-urlencoded;charset=UTF-8"}};
    char *result = NULL;
    int ret;
    snprintf(request_url, sizeof(request_url), "%s%s", url.c_str(), ELEVATOR_STATUS_PATH);
    snprintf(request, sizeof(request), 
             "appId=%s&appSecret=%s&licence=%s&deviceId=%d",
             appid.c_str(), appsecret.c_str(), licence.c_str(), elevator_id);
    LOGT(WLONG_3P_CTRL, "elevator_status url:%s, request:%s", request_url, request);
    #if !STUB_ENABLE
    ret = HttpPostWithHeadersTimeout(request_url, request, headers, 1, 5, &result);
    #else
    ret = 0;
    result = (char *)uni_malloc(1024);
    sprintf(result, "%s", "{\"code\": 0, \"message\": \"OK\", \"data\":\"no data\"}");
    #endif
    if (0 == ret &&NULL != result &&
        0 == _parse_lift_status(result, response)) {
        LOGT(WLONG_3P_CTRL, "wlong elevator_status calling succeeded");
        ret = 0;
    } else {
        LOGE(WLONG_3P_CTRL, "wlong elevator_status calling failed, ret = %d", ret);
        ret = -1;
    }
    if (NULL != result) {
        uni_free(result);
    }
    return ret;
}
