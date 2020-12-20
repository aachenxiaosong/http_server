#include "RiliBookLiftInterMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "rili_uart_protocol.h"
#include "UniLog.hpp"
#include "Poco/URI.h"

#define RILI_BOOK_LIFT_INTER_MSG_HANDLER_TAG "rili_book_lift_inter_msg_handler"

RiliBookLiftInterMessageHandler :: RiliBookLiftInterMessageHandler() : ILiftCtrlHttpMessageHandler("rili_book_lift_inter_handler")
{
}

RiliBookLiftInterMessageHandler :: ~RiliBookLiftInterMessageHandler()
{
}

LiftCtrlMessageRsp* RiliBookLiftInterMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_INTER_REQ) {
        return NULL;
    }
    const LiftCtrlMessageBookLiftInterReq& req = dynamic_cast<const LiftCtrlMessageBookLiftInterReq&>(request);
    LiftCtrlMessageBookLiftInterRsp rsp;
    //step1: 获取dHomeId获取楼栋
    string building_num = SulinkLiftInitData::getBuildingNoBySpaceId(req.dHomeId());
    //step2: 根据dHomeId获取群控器的ip和端口;并找到目的房间的房间号
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.dHomeId());
    string dhome_num = SulinkLiftInitData :: getHomeNoBySpaceId(req.dHomeId());
    //step3: 根据sHomeId找到出发房间号
    string shome_num = SulinkLiftInitData :: getHomeNoBySpaceId(req.sHomeId());
    string not_found_msg = "";
    if (building_num.empty()) {
        not_found_msg = "building number not found for home id " + req.dHomeId();
    }
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.dHomeId();
    }
    if (dhome_num.empty()) {
        not_found_msg = "dest home number not found for home id " + req.dHomeId();
    }
    if (shome_num.empty()) {
        not_found_msg = "from home number not found for home id " + req.sHomeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftInterRsp(rsp);
    }
    //step4: 调用rili远程呼梯接口
    int ret = 0;
    RiliRequestInterVisit request_inter_visit;
    RiliResponseInterVisit response_inter_visit;
    Poco::URI uri(cluster_url);
    request_inter_visit.building_num = atoi(building_num.c_str());
    snprintf(request_inter_visit.host_room, sizeof(request_inter_visit.host_room), "%s", dhome_num.c_str());
    snprintf(request_inter_visit.visitor_room, sizeof(request_inter_visit.visitor_room), "%s", shome_num.c_str());
    ret = rili_protocol_send(uri.getHost().c_str(), uri.getPort(), RILI_EVENT_INTER_VISIT, &(request_inter_visit), &(response_inter_visit));
    if (ret == 0) {
        LOGT(RILI_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of rili book lift internal OK");
        rsp.retcode(0);
        if (response_inter_visit.ack_code == 1) {
            rsp.msg("OK");
        } else {
            rsp.msg("request rejected by Rili lift ctrl system");
        }
        rsp.ackCode(response_inter_visit.ack_code);
        rsp.elevatorId(-1);
    } else {
        LOGT(RILI_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of rili book lift internal failed");
        rsp.retcode(-1);
        rsp.msg("calling Rili interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftInterRsp(rsp);
}