#include "WlongTakeLiftHttpHandler.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_TAKE_TAG "wlong_take"

WlongTakeLiftHttpHandler :: WlongTakeLiftHttpHandler() : IHttpRequestHandler("wlong_take_lift") {
    LOGT(WLONG_TAKE_TAG, "WlongTakeLift object created");
}

int WlongTakeLiftHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    if (true != jrequest.Get("buildingId", ivalue)) {
        err_field = "buildingId";
        return -1;
    }
     if (true != jrequest.Get("unitId", ivalue)) {
        err_field = "unitId";
        return -1;
    }
     if (true != jrequest.Get("elevatorId", ivalue)) {
        err_field = "elevatorId";
        return -1;
    }
     if (true != jrequest.Get("lightupFloor", svalue)) {
        err_field = "lightupFloor";
        return -1;
    }
    if (true != jrequest.Get("unlockFloors", jvalue) || true != jvalue.IsArray()) {
        err_field = "unlockFloors";
        return -1;
    }
    return 0;
}

int WlongTakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/takeLift") != 0) {
        LOGT(WLONG_TAKE_TAG, "%s is not for WlongTakeLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_TAKE_TAG, "WlongTakeLift takeed");
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        LOGT(WLONG_TAKE_TAG, "check request %s failed", request.c_str());
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "wrong param " + err_field);
        jresponse.Add("ackCode", 0);
        response = jresponse.ToString();
        return 0;
    }
    #if STUB_ENABLE
    usleep(1000 * 500);
    jresponse.Add("errCode", 0);
    jresponse.Add("errMsg", "OK");
    jresponse.Add("ackCode", 1);
    response = jresponse.ToString();
    #else
    //TODO
    #endif
    return 0;
}