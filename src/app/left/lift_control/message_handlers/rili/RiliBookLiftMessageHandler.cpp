#include "RiliBookLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "rili_uart_protocol.h"
#include "UniLog.hpp"
#include "Poco/URI.h"

#define RILI_BOOK_LIFT_MSG_HANDLER_TAG "rili_book_lift_msg_handler"

RiliBookLiftMessageHandler :: RiliBookLiftMessageHandler() : ILiftCtrlHttpMessageHandler("rili_book_lift_handler")
{
}

RiliBookLiftMessageHandler :: ~RiliBookLiftMessageHandler()
{
}

LiftCtrlMessageRsp* RiliBookLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_REQ) {
        return NULL;
    }
    LiftCtrlMessageBookLiftReq& req = (LiftCtrlMessageBookLiftReq &)request;
    LiftCtrlMessageBookLiftRsp rsp;
    //step1: 获取defaultHomeId获取楼栋
    string building_num = SulinkLiftInitData::getBuildingNoBySpaceId(req.defaultHomeId());
    //step2: 根据defaultHomeId获取群控器的ip和端口;并找到目的房间的房间号(注意日立仅支持单层解锁)
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.defaultHomeId());
    string home_num = SulinkLiftInitData :: getHomeNoBySpaceId(req.defaultHomeId());
    //step3: 根据deviceCode找到hall num
    string hall_num = SulinkLiftInitData :: getDeviceHallNo(req.deviceCode());
    string not_found_msg = "";
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.defaultHomeId();
    }
    if (home_num.empty()) {
        not_found_msg = "home number not found for home id " + req.defaultHomeId();
    }
    if (hall_num.empty()) {
        not_found_msg = "hall number not found for home id " + req.defaultHomeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    } 
    //step4: 调用rili远程呼梯接口
    int ret = 0;
    RiliRequestExterVisit request_exter_visit;
    RiliResponseExterVisit response_exter_visit;
    Poco::URI uri(cluster_url);
    request_exter_visit.building_num = atoi(building_num.c_str());
    request_exter_visit.hall_num = atoi(hall_num.c_str());
    snprintf(request_exter_visit.room, sizeof(request_exter_visit.room), "%s", home_num.c_str());
    ret = rili_protocol_send(uri.getHost().c_str(), uri.getPort(), RILI_EVENT_EXTER_VISIT, &(request_exter_visit), &(response_exter_visit));
    if (ret == 0) {
        LOGT(RILI_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of rili book lift OK");
        rsp.retcode(0);
        if (response_exter_visit.ack_code == 1) {
            rsp.msg("OK");
        } else {
            rsp.msg("request rejected by Rili lift ctrl system");
        }
        rsp.ackCode(response_exter_visit.ack_code);
        rsp.elevatorId(-1);
    } else {
        LOGT(RILI_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of rili book lift failed");
        rsp.retcode(-1);
        rsp.msg("calling Rili interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftRsp(rsp);
}
