#include "RiliLiftStatusMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "rili_uart_protocol.h"
#include "UniLog.hpp"
#include "Poco/URI.h"

#define RILI_LIFT_STATUS_MSG_HANDLER_TAG getName().c_str()

RiliLiftStatusMessageHandler :: RiliLiftStatusMessageHandler() : ILiftCtrlHttpMessageHandler("rili_lift_status_msg_handler")
{
}

RiliLiftStatusMessageHandler :: ~RiliLiftStatusMessageHandler()
{
}

LiftCtrlMessageRsp* RiliLiftStatusMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_LIFT_STATUS_REQ) {
        return NULL;
    }
    LOGT(RILI_LIFT_STATUS_MSG_HANDLER_TAG, "request message is handling...");
    const LiftCtrlMessageLiftStatusReq& req = dynamic_cast<const LiftCtrlMessageLiftStatusReq&>(request);
    LiftCtrlMessageLiftStatusRsp rsp;
    //step1: 获取homeId获取楼栋
    string building_num = SulinkLiftInitData::getBuildingNoBySpaceId(req.homeId());
    //step2: 根据homeId获取群控器的ip和端口
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    string not_found_msg = "";
    if (building_num.empty()) {
        not_found_msg = "building number not found for home id " + req.homeId();
    } else if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        LOGE(RILI_LIFT_STATUS_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageLiftStatusRsp(rsp);
    }
    //step2: 调用rili电梯状态接口
    int ret = 0;
    RiliRequestLiftStatus request_lift_status;
    RiliResponseLiftStatus response_lift_status;
    Poco::URI uri(cluster_url);
    request_lift_status.building_num = atoi(building_num.c_str());
    request_lift_status.lift_num = req.elevatorId();
    ret = rili_protocol_send(uri.getHost().c_str(), uri.getPort(), RILI_EVENT_LIFT_STATUS, &(request_lift_status), &(response_lift_status));
    if (ret == 0) {
        LOGT(RILI_LIFT_STATUS_MSG_HANDLER_TAG, "handle request of rili lfit status OK");
        rsp.retcode(0);
        rsp.msg("OK");
        rsp.curFloor(to_string(response_lift_status.current_level));
        if (response_lift_status.lift_status == 1 || response_lift_status.lift_status == 5) {
            rsp.direction("up");
        } else if (response_lift_status.lift_status == 4) {
            rsp.movingStatus("error");
        } else {
            rsp.direction("down");
        }
        rsp.doorStatus("unsupported");
        if (response_lift_status.lift_status == 1&& response_lift_status.lift_status == 2) {
            rsp.movingStatus("moving");
        } else if (response_lift_status.lift_status == 4) {
            rsp.movingStatus("error");
        } else {
            rsp.movingStatus("stopped");
        }
    } else {
        LOGT(RILI_LIFT_STATUS_MSG_HANDLER_TAG, "handle request of rili lfit status failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling Rili interface error");
    }
    return new LiftCtrlMessageLiftStatusRsp(rsp);
}