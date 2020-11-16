#include "WlongCallLiftOutHttpHandler.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_CALL_OUT_TAG getName().c_str()

WlongCallLiftOutHttpHandler :: WlongCallLiftOutHttpHandler() : IBookLiftHttpHandler("wlong_call_litf_out") {
    LOGT(WLONG_CALL_OUT_TAG, "WlongCallLiftOut object created");
}

int WlongCallLiftOutHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    /*int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time;
    CJsonObject jinfo;
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    InitInfo :: getInfo(jinfo);
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
    }*/
    return 0;
}