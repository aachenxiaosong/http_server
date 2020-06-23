#include "WlongBookLift1HttpHandler.hpp"
#include "WlongInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_BOOK1_TAG "wlong_book1"

WlongBookLift1HttpHandler :: WlongBookLift1HttpHandler() : IHttpRequestHandler("wlong_book_lift1") {
    LOGT(WLONG_BOOK1_TAG, "WlongBookLift1 object created");
}

int WlongBookLift1HttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    int ivalue;
    string svalue;
    string book_type;
    string device_type;
    if (true != jrequest.Get("bookType", book_type) ||
       (book_type.compare("exterVisit") != 0 && book_type.compare("getHome") != 0)) {
        err_field = "bookType";
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
    if (true != jrequest.Get("unlockTime", ivalue)) {
        err_field = "unlockTime";
        return -1;
    }
    return 0;
}

int WlongBookLift1HttpHandler :: _get_home_info_from_id(CJsonObject& jinfo, int home_id, CJsonObject& jhome) {
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
                    LOGT(WLONG_BOOK1_TAG, "find home %s for id %d", jhome.ToString().c_str(), home_id);
                    return 0;
                }
            }
        }
    }
    return -1;
}

int WlongBookLift1HttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
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
    string book_type;
    //prepare for wlong lift ctrl params
    jrequest.Get("dhomeId", ivalue);
    if (0 != _get_home_info_from_id(jinfo, ivalue, jhome) ) {
        LOGE(WLONG_BOOK1_TAG, "reject request for homeid not found in wlong info");
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
    jhome.Get("floor", open_floors);
    jrequest.Get("bookType", open_time);
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
    return ret;
}


int WlongBookLift1HttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift1") != 0) {
        LOGT(WLONG_BOOK1_TAG, "%s is not for WlongBookLift1", path.c_str());
        return -1;
    }
    LOGT(WLONG_BOOK1_TAG, "WlongBookLift1 booked");
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check wanglong info */
    if (WlongInfo :: getInfo().IsEmpty()) {
        LOGE(WLONG_BOOK1_TAG, "reject request for wlong info is not ready (sent from connecting platform)");
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
        LOGT(WLONG_BOOK1_TAG, "check request %s failed", request.c_str());
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
    LOGT(WLONG_BOOK1_TAG, "got response %s", response.c_str());
    return 0;
}