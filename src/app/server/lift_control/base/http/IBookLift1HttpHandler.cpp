#include "IBookLift1HttpHandler.hpp"
#include "InitInfo.hpp"
#include "uni_log.h"
#include "uni_iot.h"

#define I_BOOK1_TAG getName().c_str()

IBookLift1HttpHandler :: IBookLift1HttpHandler(const char* name) : IHttpRequestHandler(name) {
    LOGT(I_BOOK1_TAG, "object created");
}

IBookLift1HttpHandler :: ~IBookLift1HttpHandler() {}

int IBookLift1HttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
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

int IBookLift1HttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift1") != 0) {
        LOGT(I_BOOK1_TAG, "%s is not for me", path.c_str());
        return -1;
    }
    LOGT(I_BOOK1_TAG, "%s is for me", path.c_str());
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    CJsonObject jinfo;
    /* step 1, check init info */
    if (0 != InitInfo :: getInfo(jinfo)) {
        LOGE(I_BOOK1_TAG, "reject request for init info is not ready (sent from connecting platform)");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "lack of init info");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        response = jresponse.ToString();
        return 0;
    }
    /* step2, check request */
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        LOGT(I_BOOK1_TAG, "check request %s failed", request.c_str());
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
    LOGT(I_BOOK1_TAG, "got response %s", response.c_str());
    return 0;
}