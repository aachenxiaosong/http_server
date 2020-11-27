#include "IBookLift2HttpHandler.hpp"
#include "InitInfo.hpp"
#include "uni_log.h"
#include "uni_iot.h"

#define I_BOOK_TAG getName().c_str()

IBookLift2HttpHandler :: IBookLift2HttpHandler(const char* name) : IHttpRequestHandler(name) {
    LOGT(I_BOOK_TAG, "object created");
}

IBookLift2HttpHandler :: ~IBookLift2HttpHandler() {}

int IBookLift2HttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    int ivalue;
    string svalue;
    string book_type;
    string device_type;
    if (true != jrequest.Get("elevatorHallId", ivalue)) {
        err_field = "elevatorHallId";
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
    return 0;
}

int IBookLift2HttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift2") != 0) {
        LOGT(I_BOOK_TAG, "%s is not for me", path.c_str());
        return -1;
    }
    LOGT(I_BOOK_TAG, "%s is for me", path.c_str());
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    CJsonObject jinfo;
    /* step 1, check init info */
    if (0 != InitInfo :: getInfo(jinfo)) {
        LOGE(I_BOOK_TAG, "reject request for init info is not ready (sent from connecting platform)");
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
        LOGT(I_BOOK_TAG, "check request %s failed", request.c_str());
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
    LOGT(I_BOOK_TAG, "got response %s", response.c_str());
    return 0;
}
