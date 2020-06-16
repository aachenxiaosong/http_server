#include "WlongCallLiftOutHttpHandler.hpp"
#include "WlongInfo.hpp"
#include "WlongLiftCtrl.hpp"
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
     if (true != jrequest.Get("sfloorId", svalue)) {
        err_field = "sfloorId";
        return -1;
    }
     if (true != jrequest.Get("upDown", svalue) || (svalue.compare("up") != 0 && svalue.compare("down") != 0)) {
        err_field = "upDown";
        return -1;
    }
    return 0;
}

int WlongCallLiftOutHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time;
    CJsonObject jinfo = WlongInfo :: getInfo();
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    //prepare for wlong lift ctrl params
    jrequest.Get("elevatorHallId", cluster_id);
    jrequest.Get("sfloorId", from_floor);
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
        LOGT(WLONG_CALL_OUT_TAG, "handle request of wlong call lift out OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
        jresponse.Add("elevatorId", -1);
    } else {
        LOGT(WLONG_CALL_OUT_TAG, "handle request of wlong call lift out failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
    }
    return ret;
}

int WlongCallLiftOutHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift/outSide") != 0) {
        LOGT(WLONG_CALL_OUT_TAG, "%s is not for WlongCallLiftOut", path.c_str());
        return -1;
    }
    LOGT(WLONG_CALL_OUT_TAG, "WlongCallLiftOut called");
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check wanglong info */
    if (WlongInfo :: getInfo().IsEmpty()) {
        LOGE(WLONG_CALL_OUT_TAG, "reject request for wlong info is not ready (sent from connecting platform)");
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
        LOGT(WLONG_CALL_OUT_TAG, "check request %s failed", request.c_str());
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
    LOGT(WLONG_CALL_OUT_TAG, "got response %s", response.c_str());
    return 0;
}