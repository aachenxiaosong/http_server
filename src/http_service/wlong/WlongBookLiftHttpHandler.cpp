#include "WlongBookLiftHttpHandler.hpp"
#include "WlongInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_BOOK_TAG "wlong_book"

WlongBookLiftHttpHandler :: WlongBookLiftHttpHandler() : IHttpRequestHandler("wlong_book_lift") {
    LOGT(WLONG_BOOK_TAG, "WlongBookLift object created");
}

int WlongBookLiftHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    int ivalue;
    string svalue;
    string book_type;
    string device_type;
    if (true != jrequest.Get("buildingId", ivalue)) {
        err_field = "buildingId";
        return -1;
    }
    if (true != jrequest.Get("elevatorHallId", ivalue)) {
        err_field = "elevatorHallId";
        return -1;
    }
    if (true != jrequest.Get("bookType", book_type) ||
       (book_type.compare("interVisit") != 0 &&book_type.compare("exterVisit") != 0 && book_type.compare("getHome") != 0)) {
        err_field = "bookType";
        return -1;
    }
     if (true != jrequest.Get("sfloorId", svalue)) {
        err_field = "sfloorId";
        return -1;
    }
    if (true != jrequest.Get("dfloorId", svalue)) {
        err_field = "dfloorId";
        return -1;
    }
    if (book_type.compare("interVisit") == 0 && true != jrequest.Get("shomeId", ivalue)) {
        err_field = "shomeId";
        return -1;
    }
    if (true != jrequest.Get("dhomeId", ivalue)) {
        err_field = "dhomeId";
        return -1;
    }
    if (true != jrequest.Get("deviceCode", svalue)) {
        err_field = "deviceCode";
        return -1;
    }
    if (true != jrequest.Get("deviceType", device_type) ||
        (device_type.compare("unit") != 0 && device_type.compare("indoor")  != 0 &&
         device_type.compare("access") != 0 && device_type.compare("phone") != 0)) {
        err_field = "deviceType";
        return -1;
    }
    if ((book_type.compare("interVisit") == 0 || book_type.compare("exterVisit") == 0)&& true != jrequest.Get("unlockTime", ivalue)) {
        err_field = "unlockTime";
        return -1;
    }
    return 0;
}

int WlongBookLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time;
    CJsonObject jinfo = WlongInfo :: getInfo();
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    string book_type;
    //prepare for wlong lift ctrl params
    jrequest.Get("bookType", book_type);
    jrequest.Get("elevatorHallId", cluster_id);
    jrequest.Get("sfloorId", from_floor);
    updown = "2";//up
    if (book_type.compare("interVisit")) {
        if (jrequest("sfloorId").compare(jrequest("dfloorId").c_str()) > 0) {
            updown = "1";//down
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


int WlongBookLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift") != 0) {
        LOGT(WLONG_BOOK_TAG, "%s is not for WlongBookLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_BOOK_TAG, "WlongBookLift booked");
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check wanglong info */
    if (WlongInfo :: getInfo().IsEmpty()) {
        LOGE(WLONG_BOOK_TAG, "reject request for wlong info is not ready (sent from connecting platform)");
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
        LOGT(WLONG_BOOK_TAG, "check request %s failed", request.c_str());
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
    LOGT(WLONG_BOOK_TAG, "got response %s", response.c_str());
    return 0;
}