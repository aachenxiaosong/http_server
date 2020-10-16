#include "WlongTakeLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "WlongLiftCtrl.hpp"
#include "configurable_info.h"
#include "uni_log.h"
#include "uni_iot.h"

#define WLONG_TAKE_TAG getName().c_str()

WlongTakeLiftHttpHandler :: WlongTakeLiftHttpHandler() : ITakeLiftHttpHandler("wlong_take_lift") {
    LOGT(WLONG_TAKE_TAG, "WlongTakeLift object created");
}

int WlongTakeLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int lift_id;
    int i;
    int unlock_floor_num;
    string open_floors;
    string unlock_floors;
    CJsonObject jinfo;
    WlongResponse wl_response;
    int ivalue;
    string svalue;
    InitInfo :: getInfo(jinfo);
    //prepare for wlong lift ctrl params
    jrequest.Get("elevatorId", lift_id);
    jrequest.Get("lightupFloor", open_floors);
    unlock_floor_num = jrequest["unlockFloors"].GetArraySize();
    unlock_floors = "";
    for (i = 0; i < unlock_floor_num; i++) {
        unlock_floors += jrequest["unlockFloors"](i);
        if (i != (unlock_floor_num - 1)) {
            unlock_floors += ",";
        }
    }
    //prepare for wlong lift ctrl params done
    
    WlongLiftCtrl wlong_lift_ctrl(jinfo["wlong"]("intranetUrl"), jinfo["wlong"]("appId"), jinfo["wlong"]("appSecret"), jinfo["wlong"]("licence"));
    int ret = wlong_lift_ctrl.callElevatorByFoor(lift_id, open_floors, unlock_floors, wl_response);
    if (ret == 0) {
        LOGT(WLONG_TAKE_TAG, "handle request of wlong take lift OK");
        jresponse.Add("errCode", 0);
        jresponse.Add("errMsg", wl_response.msg);
        if (wl_response.code == 0) {
            jresponse.Add("ackCode", 1);
        } else {
            jresponse.Add("ackCode", 0);
        }
    } else {
        LOGT(WLONG_TAKE_TAG, "handle request of wlong take lift failed");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of 3p interface failed");
        jresponse.Add("ackCode", 0);
    }
    return 0;
}
