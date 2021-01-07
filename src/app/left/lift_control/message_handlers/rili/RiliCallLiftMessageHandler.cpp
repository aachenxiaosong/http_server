#include "RiliCallLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "rili_uart_protocol.h"
#include "UniLog.hpp"
#include "Poco/URI.h"

#define RILI_CALL_LIFT_MSG_HANDLER_TAG getName().c_str()

RiliCallLiftMessageHandler :: RiliCallLiftMessageHandler() : ILiftCtrlHttpMessageHandler("rili_call_lift_msg_handler")
{
}

RiliCallLiftMessageHandler :: ~RiliCallLiftMessageHandler()
{
}

LiftCtrlMessageRsp* RiliCallLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_CALL_LIFT_REQ) {
        return NULL;
    }
    LOGT(RILI_CALL_LIFT_MSG_HANDLER_TAG, "request message is handling...");
    const LiftCtrlMessageCallLiftReq& req = dynamic_cast<const LiftCtrlMessageCallLiftReq&>(request);
    LiftCtrlMessageCallLiftRsp rsp;
    //step1: 获取homeId获取楼栋
    string building_num = SulinkLiftInitData::getBuildingNoBySpaceId(req.homeId());
    //step2: 根据homeId获取群控器的ip和端口;并找到目的房间的房间号
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    string home_num = SulinkLiftInitData :: getHomeNoBySpaceId(req.homeId());
    
    string not_found_msg = "";
    if (building_num.empty()) {
        not_found_msg = "building number not found for home id " + req.homeId();
    } else if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    } else if (home_num.empty()) {
        not_found_msg = "home number not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        LOGE(RILI_CALL_LIFT_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageCallLiftRsp(rsp);
    }
    //step3: 调用rili召梯接口
    int ret = 0;
    RiliRequestCallLift request_call_lift;
    RiliResponseCallLift response_call_lift;
    Poco::URI uri(cluster_url);
    request_call_lift.building_num = atoi(building_num.c_str());
    snprintf(request_call_lift.room, sizeof(request_call_lift.room), "%s", home_num.c_str());
    ret = rili_protocol_send(uri.getHost().c_str(), uri.getPort(), RILI_EVENT_CALL_LIFT, &(request_call_lift), &(response_call_lift));
    if (ret == 0) {
        LOGT(RILI_CALL_LIFT_MSG_HANDLER_TAG, "handle request of rili call lift OK");
        rsp.retcode(RETCODE_OK);
        if (response_call_lift.ack_code == 1) {
            rsp.msg("OK");
        } else {
            rsp.msg("request rejected by Rili lift ctrl system");
        }
        rsp.ackCode(response_call_lift.ack_code);
        rsp.elevatorId(-1);
    } else {
        LOGT(RILI_CALL_LIFT_MSG_HANDLER_TAG, "handle request of rili call lift failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling Rili interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageCallLiftRsp(rsp);
}
