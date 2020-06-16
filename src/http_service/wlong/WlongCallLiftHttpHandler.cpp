#include "WlongCallLiftHttpHandler.hpp"
#include "WlongInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_CALL_TAG "wlong_call"

WlongCallLiftHttpHandler :: WlongCallLiftHttpHandler() : IHttpRequestHandler("wlong_call_lift") {
    LOGT(WLONG_CALL_TAG, "WlongCallLift object created");
}

int WlongCallLiftHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
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
     if (true != jrequest.Get("homeId", ivalue)) {
        err_field = "homeId";
        return -1;
    }
     if (true != jrequest.Get("upDown", svalue) || (svalue.compare("up") != 0 && svalue.compare("down") != 0)) {
        err_field = "upDown";
        return -1;
    }
    return 0;
}

int WlongCallLiftHttpHandler :: _get_home_info_from_id(CJsonObject& jinfo, int home_id, CJsonObject& jhome) {
    int i, j, k;
    int ivalue;
    string svalue;
    CJsonObject jbuilding;
    CJsonObject junit;
    for (i = 0; i < jinfo["buildings"].GetArraySize(); i++) {
        jbuilding = jinfo["buildings"][i];
        for (j = 0; j < jbuilding["units"].GetArraySize(); j++) {
            junit = jbuilding["units"][j];
            for (k = 0; k < junit["homes"].GetArraySize(); k++) {
                junit["homes"][k].Get("id", ivalue);
                if (ivalue == home_id) {
                    jhome = junit["homes"][k];
                    LOGT(WLONG_CALL_TAG, "find home %s for id %d", jhome.ToString().c_str(), home_id);
                    return 0;
                }
            }
        }
    }
    return -1;
}

int WlongCallLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time;
    CJsonObject jinfo = WlongInfo :: getInfo();
    CJsonObject jhome;
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    //prepare for wlong lift ctrl params
    jrequest.Get("homeId", ivalue);
    if (0 != _get_home_info_from_id(jinfo, ivalue, jhome) ) {
        LOGE(WLONG_CALL_TAG, "reject request for homeid not found in wlong info");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "home id not found");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        return -1;
    }
    jhome.Get("liftHall", svalue);
    cluster_id = atoi(svalue.c_str());
    jhome.Get("floor", from_floor);
    jrequest.Get("upDown", svalue);
    if (svalue.compare("up") == 0) {
        updown = "2";//up
    } else {
        updown = "1";//down
    }
    open_floors = "";
    open_time = 0;
    //prepare for wlong lift ctrl params done
    
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    int ret = wlong_lift_ctrl.bookingElevator(cluster_id, from_floor, updown, open_floors, open_time, wl_response);
    if (ret == 0) {
        LOGT(WLONG_CALL_TAG, "handle request of wlong book lift OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
        jresponse.Add("elevatorId", -1);
    } else {
        LOGT(WLONG_CALL_TAG, "handle request of wlong book lift failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
    }
    return ret;
}

int WlongCallLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift") != 0) {
        LOGT(WLONG_CALL_TAG, "%s is not for WlongCallLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_CALL_TAG, "WlongCallLift called");
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check wanglong info */
    if (WlongInfo :: getInfo().IsEmpty()) {
        LOGE(WLONG_CALL_TAG, "reject request for wlong info is not ready (sent from connecting platform)");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "lack of wanglong info");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        response = jresponse.ToString();
        return 0;
    }
    /* step2, check request */
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        LOGT(WLONG_CALL_TAG, "check request %s failed", request.c_str());
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "wrong param " + err_field);
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        response = jresponse.ToString();
        return 0;
    }
    /* step3, parse request */
    handleRequest(jrequest, jresponse);
    response = jresponse.ToString();
    LOGT(WLONG_CALL_TAG, "got response %s", response.c_str());
    return 0;
}