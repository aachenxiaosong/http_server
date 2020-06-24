#include "WlongTakeLiftMsgHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "McBroker.hpp"
#include "uni_log.h"

#define WLONG_TAKE_MSG_TAG "wlong_take_msg"

WlongTakeLiftMsgHandler :: WlongTakeLiftMsgHandler() : IMcMsgHandler("wlong_take_msg") {
    LOGT(WLONG_TAKE_MSG_TAG, "WlongTakeLiftMsgHandler is created");
}

int WlongTakeLiftMsgHandler :: _checkTakeLift(CJsonObject& jmsg, string& err_field) {
    int ivalue;
    string svalue;
    if (true != jmsg.Get("reqId", svalue)) {
        err_field = "reqId";
        return -1;
    }
    if (true != jmsg["takeLift"].Get("buildingId", ivalue)) {
        err_field = "buildingId";
        return -1;
    }
    if (true != jmsg["takeLift"].Get("unitId", ivalue)) {
        err_field = "unitId";
        return -1;
    }
    if (true != jmsg["takeLift"].Get("elevatorHallId", ivalue)) {
        err_field = "elevatorHallId";
        return -1;
    }
    if (true != jmsg["takeLift"].Get("sfloorId", svalue)) {
        err_field = "sfloorId";
        return -1;
    }
    if (true != jmsg["takeLift"].Get("dfloorId", svalue)) {
        err_field = "dfloorId";
        return -1;
    }
    return 0;
}

int WlongTakeLiftMsgHandler :: _handleTakeLift(CJsonObject& jmsg, CJsonObject& jresponse) {
    LOGT(WLONG_TAKE_MSG_TAG, "take lift msg is handled by wlong take lift msg handler");
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
    if (0 != _checkTakeLift(jmsg, err_field)) {
        jresponse.Add("code", 1);
        jresponse.Add("message", "wrong param " + err_field);
        return 0;
    }
    //TODO
    jresponse.Add("code", 1);
    jresponse.Add("message", "not support for now");    
    return 0;
}

int WlongTakeLiftMsgHandler :: handle(string& msg) {
    string svalue;
    CJsonObject jvalue;
    CJsonObject jmsg(msg);
    CJsonObject jresponse;
    if (true != jmsg.Get("msgType", svalue) || svalue.compare("LIFT_CTRL_CMD") != 0) {
        return -1;
    }
    if (true != jmsg.Get("bookLift", jvalue)) {
        return -1;
    }
    if (true != jmsg["bookLift"].Get("bookType", svalue) || svalue.compare("applyLift") != 0) {
        return -1;
    }
    _handleTakeLift(jmsg, jresponse);
    McBrokerSend(jresponse.ToString());
    return 0;
}