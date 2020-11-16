#include "ILiftStatusHttpHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "uni_log.h"
#include "uni_iot.h"

#define I_LIFT_STATUS_TAG getName().c_str()

ILiftStatusHttpHandler :: ILiftStatusHttpHandler(const char* name) : IHttpRequestHandler(name) {
    LOGT(I_LIFT_STATUS_TAG, "object created");
}

ILiftStatusHttpHandler :: ~ILiftStatusHttpHandler() {}

int ILiftStatusHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    int ivalue;
    if (true != jrequest.Get("elevatorId ", ivalue)) {
        err_field = "elevatorId ";
        return -1;
    }
    return 0;
}

void ILiftStatusHttpHandler :: assambleResponse(int ret_code, const string& msg, const string& cur_floor, const string& direction,
                      const string& moving_status, const string& door_status, CJsonObject& jresponse)
{
    jresponse.Add("retcode", ret_code);
    jresponse.Add("msg", msg);
    CJsonObject jdata;
    jdata.Add("curFloor", cur_floor);
    jdata.Add("direction", direction);
    jdata.Add("movingStatus", moving_status);
    jdata.Add("doorStatus", door_status);
    jresponse.Add("data", jdata);
}

int ILiftStatusHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v3/liftStatus") != 0) {
        LOGT(I_LIFT_STATUS_TAG, "%s is not for me", path.c_str());
        return -1;
    }
    LOGT(I_LIFT_STATUS_TAG, "%s is for me", path.c_str());
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check init info */
    if (true != SulinkLiftInitData :: inited()) {
        string msg = "lift init info absent";
        LOGE(I_LIFT_STATUS_TAG, "reject request %s for %s", request.c_str(), msg.c_str());
        assambleResponse(-1, msg, "", "", "", "", jresponse);
        response = jresponse.ToString();
        return 0;
        return 0;
    }
    /* step2, check request */
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
         string msg = "wrong param " + err_field;
        LOGE(I_LIFT_STATUS_TAG, "reject request %s for %s", request.c_str(), msg.c_str());
        assambleResponse(-1, msg, "", "", "", "", jresponse);
        response = jresponse.ToString();
        return 0;
    }
   /* step3, parse request */
    handleRequest(jrequest, jresponse);
    response = jresponse.ToString();
    LOGT(I_LIFT_STATUS_TAG, "got response %s", response.c_str());
    return 0;
}
