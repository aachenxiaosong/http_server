#include "WlongBookLiftHttpHandler.hpp"
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

int WlongBookLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift") != 0) {
        LOGT(WLONG_BOOK_TAG, "%s is not for WlongBookLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_BOOK_TAG, "WlongBookLift booked");
    CJsonObject jrequest(request);
    CJsonObject jresponse;
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
    #if STUB_ENABLE
    usleep(1000 * 500);
    jresponse.Add("errCode", 0);
    jresponse.Add("errMsg", "OK");
    if (rand() % 2 == 0) {
        jresponse.Add("ackCode", 1);
    } else {
        jresponse.Add("ackCode", 0);
    }
    jresponse.Add("elevatorId", -1);
    response = jresponse.ToString();
    #else
    //TODO
    #endif
    return 0;
}