#include "WlongCallLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_CALL_TAG getName().c_str()

WlongCallLiftHttpHandler :: WlongCallLiftHttpHandler() : ICallLiftHttpHandler("wlong_call_lift") {
    LOGT(WLONG_CALL_TAG, "WlongCallLift object created");
}

int WlongCallLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int cluster_id;
    string from_floor;
    string updown;
    string open_floors;
    int open_time;
    CJsonObject jinfo;
    CJsonObject jhome;
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    InitInfo :: getInfo(jinfo);
    //prepare for wlong lift ctrl params
    jrequest.Get("homeId", ivalue);
    if (0 != InitInfo :: getHome(ivalue, jhome) ) {
        LOGE(WLONG_CALL_TAG, "reject request for homeid not found in init info");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "home id not found");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        return -1;
    }
    jhome.Get("liftHall", svalue);
    cluster_id = atoi(svalue.c_str());
    jhome.Get("floor", from_floor);
    jrequest.Get("upDown", svalue);
    if (svalue.compare("up") == 0) {
        updown = "2";//up
    } else {
        updown = "1";//down
    }
    open_floors = "";
    open_time = 0;
    //prepare for wlong lift ctrl params done
    
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    int ret = wlong_lift_ctrl.bookingElevator(cluster_id, from_floor, updown, open_floors, open_time, wl_response);
    if (ret == 0) {
        LOGT(WLONG_CALL_TAG, "handle request of wlong call lift OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
        jresponse.Add("elevatorId", -1);
    } else {
        LOGT(WLONG_CALL_TAG, "handle request of wlong call lift failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
    }
    return 0;
}
