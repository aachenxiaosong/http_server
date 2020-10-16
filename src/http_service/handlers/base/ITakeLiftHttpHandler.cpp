#include "ITakeLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "uni_log.h"
#include "uni_iot.h"

#define I_TAKE_TAG getName().c_str()

ITakeLiftHttpHandler :: ITakeLiftHttpHandler(const char* name) : IHttpRequestHandler(name) {
    LOGT(I_TAKE_TAG, "object created");
}

ITakeLiftHttpHandler :: ~ITakeLiftHttpHandler() {}

int ITakeLiftHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    if (true != jrequest.Get("buildingId", ivalue)) {
        err_field = "buildingId";
        return -1;
    }
    if (true != jrequest.Get("unitId", ivalue)) {
        err_field = "unitId";
        return -1;
    }
    if (true != jrequest.Get("elevatorId", ivalue)) {
        err_field = "elevatorId";
        return -1;
    }
    /*if (true != jrequest.Get("lightupFloor", svalue)) {
        err_field = "lightupFloor";
        return -1;
    }*/
    if (true != jrequest.Get("unlockFloors", jvalue) || true != jvalue.IsArray()) {
        err_field = "unlockFloors";
        return -1;
    }
    return 0;
}

int ITakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/takeLift") != 0) {
        LOGT(I_TAKE_TAG, "%s is not for me", path.c_str());
        return -1;
    }
    LOGT(I_TAKE_TAG, "%s is for me", path.c_str());
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    CJsonObject jinfo;
    /* step 1, check init info */
    if (0 != InitInfo :: getInfo(jinfo)) {
        LOGE(I_TAKE_TAG, "reject request for init info is not ready (sent from connecting platform)");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "lack of init info");
        jresponse.Add("ackCode", 0);
        response = jresponse.ToString();
        return 0;
    }
    /* step2, check request */
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        LOGT(I_TAKE_TAG, "check request %s failed", request.c_str());
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "wrong param " + err_field);
        jresponse.Add("ackCode", 0);
        response = jresponse.ToString();
        return 0;
    }
    /* step3, parse request */
    handleRequest(jrequest, jresponse);
    response = jresponse.ToString();
    LOGT(I_TAKE_TAG, "got response %s", response.c_str());
    return 0;
}
