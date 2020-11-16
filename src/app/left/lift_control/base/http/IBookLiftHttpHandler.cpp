#include "IBookLiftHttpHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "uni_log.h"
#include "uni_iot.h"

#define I_BOOK_TAG getName().c_str()

IBookLiftHttpHandler :: IBookLiftHttpHandler(const char* name) : IHttpRequestHandler(name) {
    LOGT(I_BOOK_TAG, "object created");
}

IBookLiftHttpHandler :: ~IBookLiftHttpHandler() {}

int IBookLiftHttpHandler :: checkRqeust(CJsonObject& jrequest, string& err_field) {
    string svalue;
    if (true != jrequest.Get("homeId", svalue)) {
        err_field = "homeId";
        return -1;
    }
    /*if (true != jrequest.Get("deviceCode", svalue)) {
        err_field = "deviceCode";
        return -1;
    }*/
    return 0;
}

void IBookLiftHttpHandler :: assambleResponse(int ret_code, const string& msg, int ack_code, int elevator_id, CJsonObject& jresponse)
{
    jresponse.Add("retcode", ret_code);
    jresponse.Add("msg", msg);
    CJsonObject jdata;
    jdata.Add("ackCode", ack_code);
    jdata.Add("elevatorId", elevator_id);
    jresponse.Add("data", jdata);
}


int IBookLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v3/bookLift") != 0) {
        LOGT(I_BOOK_TAG, "%s is not for me", path.c_str());
        return -1;
    }
    LOGT(I_BOOK_TAG, "%s is for me", path.c_str());
    /* process request */
    CJsonObject jrequest(request);
    CJsonObject jresponse;
    /* step 1, check init info */
    if (true != SulinkLiftInitData :: inited()) {
        string msg = "lift init info absent";
        LOGE(I_BOOK_TAG, "reject request %s for %s", request.c_str(), msg.c_str());
        assambleResponse(-1, msg, 0, -1, jresponse);
        response = jresponse.ToString();
        return 0;
    }
    /* step2, check request */
    string err_field = "";
    if (0 != checkRqeust(jrequest, err_field)) {
        string msg = "wrong param " + err_field;
        LOGE(I_BOOK_TAG, "reject request %s for %s", request.c_str(), msg.c_str());
        assambleResponse(-1, msg, 0, -1, jresponse);
        response = jresponse.ToString();
        return 0;
    }
    /* step3, parse request */
    handleRequest(jrequest, jresponse);
    response = jresponse.ToString();
    LOGT(I_BOOK_TAG, "got response %s", response.c_str());
    return 0;
}
