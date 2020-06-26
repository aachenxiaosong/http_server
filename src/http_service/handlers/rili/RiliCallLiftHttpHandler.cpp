#include "RiliCallLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "rili_uart_protocol.h"
#include "uni_log.h"

#define RILI_CALL_TAG getName().c_str()

RiliCallLiftHttpHandler :: RiliCallLiftHttpHandler() : ICallLiftHttpHandler("rili_call_lift") {
    LOGT(RILI_CALL_TAG, "RiliCallLift object created");
}
int RiliCallLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    int ivalue;
    string svalue;
    CJsonObject jbuilding;
    CJsonObject jdhome;
    int building_num;
    char droom[RILI_ROOM_LENGTH] = {0};
    //param 1，building_id
    jrequest.Get("buildingId", ivalue);
    if (0 != InitInfo :: getBuilding(ivalue, jbuilding)) {
        LOGE(RILI_CALL_TAG, "reject request for buildingid not found in init info");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "building id not found");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        //return -1;
        return 0;
    }
    jbuilding.Get("number", svalue);
    building_num = atoi(svalue.c_str());
    //param 2, host_room
    jrequest.Get("homeId", ivalue);
    if (0 != InitInfo :: getHome(ivalue, jbuilding, jdhome)) {
        LOGE(RILI_CALL_TAG, "reject request for dhome not found in init info");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "dhome id not found");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        //return -1;
        return 0;
    }
    jdhome.Get("number", svalue);
    snprintf(droom, sizeof(droom), "%40d", atoi(svalue.c_str())); //不够4位，前面补0
    //uart 串口请求
    int ret = 0;
    int ack_code;
    RiliRequestCallLift request_call_lift;
    RiliResponseCallLift response_call_lift;
    request_call_lift.building_num = building_num;
    snprintf(request_call_lift.room, sizeof(request_call_lift.room), "%s", droom);
    ret = rili_protocol_send(RILI_EVENT_CALL_LIFT, &(request_call_lift), &(response_call_lift));
    ack_code = response_call_lift.ack_code;
    if (0 == ret)
    {
        jresponse.Add("errCode", 0);
        if (1 == ack_code)
        {
            jresponse.Add("errMsg", "OK");
            jresponse.Add("ackCode", 1);
            jresponse.Add("elevatorId", -1);
        }
        else
        {
            jresponse.Add("errMsg", "request of Rili 3p interface rejected");
            jresponse.Add("ackCode", 0);
            jresponse.Add("elevatorId", -1);
        }
    }
    else
    {
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "request of Rili 3p interface failed");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
    }
    return 0;
}