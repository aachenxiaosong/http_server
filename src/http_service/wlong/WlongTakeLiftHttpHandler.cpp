#include "WlongTakeLiftHttpHandler.hpp"
#include "WlongInfo.hpp"
#include "WlongLiftCtrl.hpp"
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

int WlongTakeLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int lift_id;
    int i;
    int unlock_floor_num;
    string open_floors;
    string unlock_floors;
    CJsonObject jinfo = WlongInfo :: getInfo();
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    //prepare for wlong lift ctrl params
    jrequest.Get("elevatorId", lift_id);
    jrequest.Get("lightupFloor", open_floors);
    unlock_floor_num = jrequest["unlockFloors"].GetArraySize();
    unlock_floors = "";
    for (i = 0; i < unlock_floor_num; i++) {
        unlock_floors += jrequest["unlockFloors"](i);
        if (i != (unlock_floor_num - 1)) {
            unlock_floors += ",";
        }
    }
    //prepare for wlong lift ctrl params done
    
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    int ret = wlong_lift_ctrl.callElevatorByFoor(lift_id, open_floors, unlock_floors, wl_response);
    if (ret == 0) {
        LOGT(WLONG_TAKE_TAG, "handle request of wlong take lift OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
    } else {
        LOGT(WLONG_TAKE_TAG, "handle request of wlong take lift failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
    }
    return ret;
}

int WlongTakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/takeLift") != 0) {
        LOGT(WLONG_TAKE_TAG, "%s is not for WlongTakeLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_TAKE_TAG, "WlongTakeLift called");
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check wanglong info */
    if (WlongInfo :: getInfo().IsEmpty()) {
        LOGE(WLONG_TAKE_TAG, "reject request for wlong info is not ready (sent from connecting platform)");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "lack of wanglong info");
        jresponse.Add("ackCode", 0);
        response = jresponse.ToString();
        return 0;
    }
    /* step2, check request */
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        LOGT(WLONG_TAKE_TAG, "check request %s failed", request.c_str());
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "wrong param " + err_field);
        jresponse.Add("ackCode", 0);
        response = jresponse.ToString();
        return 0;
    }
    /* step3, parse request */
    handleRequest(jrequest, jresponse);
    response = jresponse.ToString();
    LOGT(WLONG_TAKE_TAG, "got response %s", response.c_str());
    return 0;
}