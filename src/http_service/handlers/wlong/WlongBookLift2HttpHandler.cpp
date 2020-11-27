#include "WlongBookLift2HttpHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_BOOK_TAG getName().c_str()

WlongBookLift2HttpHandler :: WlongBookLift2HttpHandler() : IBookLift2HttpHandler("wlong_book_lift2") {
    LOGT(WLONG_BOOK_TAG, "WlongBookLift2 object created");
}

int WlongBookLift2HttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int cluster_id;
    string from_floor;
    string updown;
    string open_floor;
    CJsonObject jinfo;
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    InitInfo :: getInfo(jinfo);
    //prepare for wlong lift ctrl params
    jrequest.Get("elevatorHallId", cluster_id);
    jrequest.Get("sfloorId", from_floor);
    updown = WLONG_UP;//up
    if (jrequest("sfloorId").compare(jrequest("dfloorId").c_str()) > 0)
    {
        updown = WLONG_DN; //down
    }
    open_floor = jrequest("dfloorId");
    //prepare for wlong lift ctrl params done
    
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    int ret = wlong_lift_ctrl.reserveElevator(cluster_id, from_floor, open_floor, wl_response);
    if (ret == 0) {
        LOGT(WLONG_BOOK_TAG, "handle request of wlong book lift2 OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
        jresponse.Add("elevatorId", -1);
    } else {
        LOGT(WLONG_BOOK_TAG, "handle request of wlong book lift2 failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
    }
    return 0;
}
