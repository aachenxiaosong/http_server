#include "ICallLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "uni_log.h"
#include "uni_iot.h"

#define I_CALL_TAG getName().c_str()

ICallLiftHttpHandler :: ICallLiftHttpHandler(const char* name) : IHttpRequestHandler(name) {
    LOGT(I_CALL_TAG, "object created");
}

ICallLiftHttpHandler :: ~ICallLiftHttpHandler() {}

int ICallLiftHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
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
    if (true != jrequest.Get("homeId", ivalue)) {
        err_field = "homeId";
        return -1;
    }
    if (true != jrequest.Get("upDown", svalue) || (svalue.compare("up") != 0 && svalue.compare("down") != 0)) {
        err_field = "upDown";
        return -1;
    }
    return 0;
}

int ICallLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift") != 0) {
        LOGT(I_CALL_TAG, "%s is not for me", path.c_str());
        return -1;
    }
    LOGT(I_CALL_TAG, "%s is for me", path.c_str());
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    CJsonObject jinfo;
    /* step 1, check init info */
    if (0 != InitInfo :: getInfo(jinfo)) {
        LOGE(I_CALL_TAG, "reject request for init info is not ready (sent from connecting platform)");
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
        LOGT(I_CALL_TAG, "check request %s failed", request.c_str());
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
    LOGT(I_CALL_TAG, "got response %s", response.c_str());
    return 0;
}
