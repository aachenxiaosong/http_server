#include "ITakeLiftHttpHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "uni_log.h"
#include "uni_iot.h"

#define I_TAKE_TAG getName().c_str()

ITakeLiftHttpHandler :: ITakeLiftHttpHandler(const char* name) : IHttpRequestHandler(name) {
    LOGT(I_TAKE_TAG, "object created");
}

ITakeLiftHttpHandler :: ~ITakeLiftHttpHandler() {}

int ITakeLiftHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    string svalue;
    if (true != jrequest.Get("defaultHomeId", svalue)) {
        err_field = "defaultHomeId";
        return -1;
    }
    /*if (true != jrequest.Get("homeIds", jvalue) || true != jvalue.IsArray()) {
        err_field = "homeIds";
        return -1;
    }*/
    if (true != jrequest.Get("deviceCode", svalue)) {
        err_field = "deviceCode";
        return -1;
    }
    return 0;
}

void ITakeLiftHttpHandler :: assambleResponse(int ret_code, const string& msg, int ack_code, CJsonObject& jresponse)
{
    jresponse.Add("retcode", ret_code);
    jresponse.Add("msg", msg);
    CJsonObject jdata;
    jdata.Add("ackCode", ack_code);
    jresponse.Add("data", jdata);
}

int ITakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v3/takeLift") != 0) {
        LOGT(I_TAKE_TAG, "%s is not for me", path.c_str());
        return -1;
    }
    LOGT(I_TAKE_TAG, "%s is for me", path.c_str());
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check init info */
    if (true != SulinkLiftInitData :: inited()) {
        string msg = "lift init info absent";
        LOGE(I_TAKE_TAG, "reject request %s for %s", request.c_str(), msg.c_str());
        assambleResponse(-1, msg, 0, jresponse);
        response = jresponse.ToString();
        return 0;
    }
    /* step2, check request */
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        string msg = "wrong param " + err_field;
        LOGE(I_TAKE_TAG, "reject request %s for %s", request.c_str(), msg.c_str());
        assambleResponse(-1, msg, 0, jresponse);
        response = jresponse.ToString();
        return 0;
    }
    /* step3, parse request */
    handleRequest(jrequest, jresponse);
    response = jresponse.ToString();
    LOGT(I_TAKE_TAG, "got response %s", response.c_str());
    return 0;
}
