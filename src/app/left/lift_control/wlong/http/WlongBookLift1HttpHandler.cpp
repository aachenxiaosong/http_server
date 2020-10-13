#include "WlongBookLift1HttpHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_BOOK1_TAG getName().c_str()

WlongBookLift1HttpHandler :: WlongBookLift1HttpHandler() : IBookLift1HttpHandler("wlong_book_lift1") {
    LOGT(WLONG_BOOK1_TAG, "WlongBookLift1 object created");
}

int WlongBookLift1HttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time = 0;
    CJsonObject jinfo;
    CJsonObject jhome;
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    string book_type;
    InitInfo :: getInfo(jinfo);
    //prepare for wlong lift ctrl params
    jrequest.Get("dhomeId", ivalue);
    if (0 != InitInfo :: getHome(ivalue, jhome) ) {
        LOGE(WLONG_BOOK1_TAG, "reject request for homeid not found in init info");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "home id not found");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        return -1;
    }
    jhome.Get("liftHall", svalue);
    cluster_id = atoi(svalue.c_str());
    jrequest.Get("bookType", book_type);
    from_floor = "1";
    updown = WLONG_UP;//up
    jrequest.Get("unlockTime", open_time);
    if (open_time < 0) {
        open_floors="";
        open_time = 0;
    } else {
        jhome.Get("floor", open_floors);
    }
    //prepare for wlong lift ctrl params done
    
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    int ret = wlong_lift_ctrl.bookingElevator(cluster_id, from_floor, updown, open_floors, open_time, wl_response);
    if (ret == 0) {
        LOGT(WLONG_BOOK1_TAG, "handle request of wlong book lift OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
        jresponse.Add("elevatorId", -1);
    } else {
        LOGT(WLONG_BOOK1_TAG, "handle request of wlong book lift failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
    }
    return 0;
}
