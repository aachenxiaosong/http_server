#include "WlongCallLiftOutHttpHandler.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_CALL_OUT_TAG "wlong_call_out"

WlongCallLiftOutHttpHandler :: WlongCallLiftOutHttpHandler() : IHttpRequestHandler("wlong_call_litf_out") {
    LOGT(WLONG_CALL_OUT_TAG, "WlongCallLiftOut object created");
}

int WlongCallLiftOutHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    int ivalue;
    string svalue;
    if (true != jrequest.Get("buildingId", ivalue)) {
        err_field = "buildingId";
        return -1;
    }
     if (true != jrequest.Get("unitId", ivalue)) {
        err_field = "unitId";
        return -1;
    }
     if (true != jrequest.Get("elevatorHallId", ivalue)) {
        err_field = "elevatorHallId";
        return -1;
    }
     if (true != jrequest.Get("upDown", svalue) || (svalue.compare("up") != 0 && svalue.compare("down") != 0)) {
        err_field = "upDown";
        return -1;
    }
    return 0;
}

int WlongCallLiftOutHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift/outSide") != 0) {
        LOGT(WLONG_CALL_OUT_TAG, "%s is not for WlongCallLiftOut", path.c_str());
        return -1;
    }
    LOGT(WLONG_CALL_OUT_TAG, "WlongCallLiftOut called");
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        LOGT(WLONG_CALL_OUT_TAG, "check request %s failed", request.c_str());
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "wrong param " + err_field);
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        response = jresponse.ToString();
        return 0;
    }
    #if STUB_ENABLE
    usleep(1000 * 500);
    jresponse.Add("errCode", 0);
    jresponse.Add("errMsg", "OK");
    jresponse.Add("ackCode", 1);
    jresponse.Add("elevatorId", -1);
    response = jresponse.ToString();
    #else
    //TODO
    #endif
    return 0;
}