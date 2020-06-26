#include "RiliBookLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "rili_uart_protocol.h"
#include "uni_log.h"

#define RILI_BOOK_TAG getName().c_str()

RiliBookLiftHttpHandler :: RiliBookLiftHttpHandler() : IBookLiftHttpHandler("rili_book_lift") {
    LOGT(RILI_BOOK_TAG, "RiliBookLift object created");
}

int RiliBookLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    string book_type;
    int ivalue;
    string svalue;
    CJsonObject jbuilding;
    CJsonObject jshome;
    CJsonObject jdhome;
    int building_num;
    int hall_num;
    char sroom[RILI_ROOM_LENGTH] = {0};
    char droom[RILI_ROOM_LENGTH] = {0};
    jrequest.Get("bookType", book_type);
    //param 1，building_id
    jrequest.Get("buildingId", ivalue);
    if (0 != InitInfo :: getBuilding(ivalue, jbuilding)) {
        LOGE(RILI_BOOK_TAG, "reject request for buildingid not found in init info");
        jresponse.Add("errCode", 1);
        jresponse.Add("errMsg", "building id not found");
        jresponse.Add("ackCode", 0);
        jresponse.Add("elevatorId", -1);
        //return -1;
        return 0;
    }
    jbuilding.Get("number", svalue);
    building_num = atoi(svalue.c_str());
    if (0 == book_type.compare("interVisit")) {
        //param 2, visitor_room for internal visit
        jrequest.Get("shomeId", ivalue);
        if (0 != InitInfo ::getHome(ivalue, jbuilding, jshome)) {
            LOGE(RILI_BOOK_TAG, "reject request for shome not found in init info");
            jresponse.Add("errCode", 1);
            jresponse.Add("errMsg", "shome id not found");
            jresponse.Add("ackCode", 0);
            jresponse.Add("elevatorId", -1);
            //return -1;
            return 0;
        }
        jshome.Get("number", svalue);
        snprintf(sroom, sizeof(sroom), "%40d", atoi(svalue.c_str())); //不够4位，前面补0
    } else {
        //param 2, hall_id for external visit or getting home
        jrequest.Get("elevatorHallId", ivalue);
        hall_num = ivalue;
    }
    //param 3, host_room
    jrequest.Get("dhomeId", ivalue);
    if (0 != InitInfo :: getHome(ivalue, jbuilding, jdhome)) {
        LOGE(RILI_BOOK_TAG, "reject request for dhome not found in init");
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
    if (0 == book_type.compare("interVisit")) {
        RiliRequestInterVisit request_inter_visit;
        RiliResponseInterVisit response_inter_visit;
        request_inter_visit.building_num = building_num;
        snprintf(request_inter_visit.host_room, sizeof(request_inter_visit.host_room), "%s", droom);
        snprintf(request_inter_visit.visitor_room, sizeof(request_inter_visit.visitor_room), "%s", sroom);
        ret = rili_protocol_send(RILI_EVENT_INTER_VISIT, &(request_inter_visit), &(response_inter_visit));
        ack_code = response_inter_visit.ack_code;
    } else if (0 == book_type.compare("exterVisit")) {
        RiliRequestExterVisit request_exter_visit;
        RiliResponseExterVisit response_exter_visit;
        request_exter_visit.building_num = building_num;
        request_exter_visit.hall_num = hall_num;
        snprintf(request_exter_visit.room, sizeof(request_exter_visit.room), "%s", droom);
        ret = rili_protocol_send(RILI_EVENT_EXTER_VISIT, &(request_exter_visit), &(response_exter_visit));
        ack_code = response_exter_visit.ack_code;
    } else {
        RiliRequestGetHome request_get_home;
        RiliResponseGetHome response_get_home;
        request_get_home.building_num = building_num;
        request_get_home.hall_num = hall_num;
        snprintf(request_get_home.room, sizeof(request_get_home.room), "%s", droom);
        ret = rili_protocol_send(RILI_EVENT_GET_HOME, &(request_get_home), &(response_get_home));
        ack_code = response_get_home.ack_code;
    }
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