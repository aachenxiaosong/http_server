#include "WlongBookLiftMsgHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "McBroker.hpp"
#include "uni_log.h"

#define WLONG_BOOK_MSG_TAG "wlong_book_msg"

WlongBookLiftMsgHandler :: WlongBookLiftMsgHandler() : IMcMsgHandler("wlong_book_msg") {
    LOGT(WLONG_BOOK_MSG_TAG, "WlongBookLiftMsgHandler is created");
}

int WlongBookLiftMsgHandler :: _checkBookLift(CJsonObject& jmsg, string& err_field) {
    int ivalue;
    string svalue;
    if (true != jmsg.Get("reqId", svalue)) {
        err_field = "reqId";
        return -1;
    }
    if (true != jmsg["bookLift"].Get("buildingId", ivalue)) {
        err_field = "buildingId";
        return -1;
    }
    if (true != jmsg["bookLift"].Get("unitId", ivalue)) {
        err_field = "unitId";
        return -1;
    }
    if (true != jmsg["bookLift"].Get("elevatorHallId", ivalue)) {
        err_field = "elevatorHallId";
        return -1;
    }
    if (true != jmsg["bookLift"].Get("ufloorId", svalue)) {
        err_field = "ufloorId";
        return -1;
    }
     if (true != jmsg["bookLift"].Get("unlockTime", ivalue)) {
        err_field = "unlockTime";
        return -1;
    }
    return 0;
}

int WlongBookLiftMsgHandler :: _handleBookLift(CJsonObject& jmsg, CJsonObject& jresponse) {
    LOGT(WLONG_BOOK_MSG_TAG, "book lift msg is handled by wlong book lift msg handler");
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
    if (0 != _checkBookLift(jmsg, err_field)) {
        LOGE(WLONG_BOOK_MSG_TAG, "check book lift msg field %s error", err_field.c_str());
        jresponse.Add("code", 1);
        jresponse.Add("message", "wrong param " + err_field);
        return 0;
    }
    //handle msg
    int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time;
    WlongResponse wl_response;
    CJsonObject jinfo = InitInfo :: getInfo();
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    jmsg["bookLift"].Get("elevatorHallId", cluster_id);
    from_floor = "1";
    updown = WLONG_UP;
    jmsg["bookLift"].Get("ufloorId", open_floors);
    jmsg["bookLift"].Get("unlockTime", open_time);
    int ret = wlong_lift_ctrl.bookingElevator(cluster_id, from_floor, updown, open_floors, open_time, wl_response);
    if (ret == 0) {
        LOGT(WLONG_BOOK_MSG_TAG, "handle msg of wlong book lift OK");
        if (wl_response.code == 0) {
            jresponse.Add("code", 1);
        } else {
            jresponse.Add("code", 0);
        }
        jresponse.Add("message", wl_response.msg);
    } else {
        LOGT(WLONG_BOOK_MSG_TAG, "handle msg of wlong book lift failed");
        jresponse.Add("code", 1);
        jresponse.Add("message", "request of 3p interface failed");
    }
    return 0;
}

int WlongBookLiftMsgHandler :: handle(string& msg) {
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
    if (true != jmsg["bookLift"].Get("bookType", svalue) || svalue.compare("unlockFloorButton") != 0) {
        return -1;
    }
    _handleBookLift(jmsg, jresponse);
    McBrokerSend(jresponse.ToString());
    return 0;
}