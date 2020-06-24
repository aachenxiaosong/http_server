#include "WlongCallLiftMsgHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "McBroker.hpp"
#include "uni_log.h"

#define WLONG_CALL_MSG_TAG "wlong_call_msg"

WlongCallLiftMsgHandler :: WlongCallLiftMsgHandler() : IMcMsgHandler("wlong_call_msg") {
    LOGT(WLONG_CALL_MSG_TAG, "WlongCallLiftMsgHandler is created");
}

int WlongCallLiftMsgHandler :: _checkCallLift(CJsonObject& jmsg, string& err_field) {
    int ivalue;
    string svalue;
    if (true != jmsg.Get("reqId", svalue)) {
        err_field = "reqId";
        return -1;
    }
    if (true != jmsg["callLift"].Get("buildingId", ivalue)) {
        err_field = "buildingId";
        return -1;
    }
    if (true != jmsg["callLift"].Get("unitId", ivalue)) {
        err_field = "unitId";
        return -1;
    }
    if (true != jmsg["callLift"].Get("homeId", ivalue)) {
        err_field = "homeId";
        return -1;
    }
    if (true != jmsg["callLift"].Get("upDown", svalue)) {
        err_field = "upDown";
        return -1;
    }
    return 0;
}

int WlongCallLiftMsgHandler :: _handleCallLift(CJsonObject& jmsg, CJsonObject& jresponse) {
    LOGT(WLONG_CALL_MSG_TAG, "call lift msg is handled by wlong call lift msg handler");
    string svalue;
    string req_id;
    string err_field;
    char timestamp[16];
    time_t timeval;
    time(&timeval);
    snprintf(timestamp, sizeof(timestamp), "%d000", (unsigned int)timeval);
    jmsg.Get("reqId", req_id);
    jresponse.Add("reqId", req_id);
    jresponse.Add("timestamp", timestamp);
    jresponse.Add("msgType", "LIFT_CTRL_STATE_RES");
    if (0 != _checkCallLift(jmsg, err_field)) {
        jresponse.Add("code", 1);
        jresponse.Add("message", "wrong param " + err_field);
        return 0;
    }
    //TODO
    jresponse.Add("code", 1);
    jresponse.Add("message", "not support for now");    
    return 0;
}

int WlongCallLiftMsgHandler :: handle(string& msg) {
    string svalue;
    CJsonObject jvalue;
    CJsonObject jmsg(msg);
    CJsonObject jresponse;
    if (true != jmsg.Get("msgType", svalue) || svalue.compare("LIFT_CTRL_CMD") != 0) {
        return -1;
    }
    if (true != jmsg.Get("callLift", jvalue)) {
        return -1;
    }
    _handleCallLift(jmsg, jresponse);
    McBrokerSend(jresponse.ToString());
    return 0;
}