#include "WlongBookLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_BOOK_TAG getName().c_str()

WlongBookLiftHttpHandler :: WlongBookLiftHttpHandler() : IBookLiftHttpHandler("wlong_book_lift") {
    LOGT(WLONG_BOOK_TAG, "WlongBookLift object created");
}

int WlongBookLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time;
    CJsonObject jinfo = InitInfo :: getInfo();
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    string book_type;
    //prepare for wlong lift ctrl params
    jrequest.Get("bookType", book_type);
    jrequest.Get("elevatorHallId", cluster_id);
    jrequest.Get("sfloorId", from_floor);
    updown = WLONG_UP;//up
    if (book_type.compare("interVisit")) {
        if (jrequest("sfloorId").compare(jrequest("dfloorId").c_str()) > 0) {
            updown = WLONG_DN;//down
        }
    }
    open_floors = jrequest("dfloorId");
    jrequest.Get("unlockTime", open_time);
    //prepare for wlong lift ctrl params done
    
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    int ret = wlong_lift_ctrl.bookingElevator(cluster_id, from_floor, updown, open_floors, open_time, wl_response);
    if (ret == 0) {
        LOGT(WLONG_BOOK_TAG, "handle request of wlong book lift OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
        jresponse.Add("elevatorId", -1);
    } else {
        LOGT(WLONG_BOOK_TAG, "handle request of wlong book lift failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
    }
    return ret;
}