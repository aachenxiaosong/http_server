#include "SlingCallLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"

#define SLING_CALL_LIFT_MSG_HANDLER_TAG "sling_call_lift_msg_handler"

SlingCallLiftMessageHandler :: SlingCallLiftMessageHandler() : ILiftCtrlHttpMessageHandler("sling_call_lift_handler")
{
}

SlingCallLiftMessageHandler :: ~SlingCallLiftMessageHandler()
{
}

LiftCtrlMessageRsp* SlingCallLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_CALL_LIFT_REQ) {
        return NULL;
    }
    LOGT(SLING_CALL_LIFT_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageCallLiftReq& req = (LiftCtrlMessageCallLiftReq &)request;
    LiftCtrlMessageCallLiftRsp rsp;
    //step1: 根据homeId获取群控器的ip和端口
    string cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.homeId());
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    //step2: 根据homeId找到出发楼层，到达楼层设置为1
    string from_floor_str = SulinkLiftInitData :: getFloorNoBySpaceId(req.homeId());
    string not_found_msg = "";
    if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for home id " + req.homeId();
    } else if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    } else if (from_floor_str.empty()) {
        not_found_msg = "from floor not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        LOGE(SLING_CALL_LIFT_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageCallLiftRsp(rsp);
    }
    //TODO: 只支持到达1楼，updown参数无效了
    string to_floor_str = unisound::UniConfig::getString("liftcontrl.groundfloor");
    if (to_floor_str.empty()) {
        to_floor_str = "1";
    }  
    SlingFloor to_floor, from_floor;
    to_floor.floor = atoi(to_floor_str.c_str());
    from_floor.floor = atoi(to_floor_str.c_str());
    //step3: 调用sling远程呼梯接口
    SlingRequestAttribute sling_req;
    SlingResponse sling_rsp;
    sling_req.seqNum = rand() % 256;
    sling_req.clusterId = atoi(cluster_id.c_str());
    sling_req.verificationLocation = sling_req.GATE;
    sling_req.verificationType = sling_req.OUTOF_CAR;
    sling_req.callAttribute = sling_req.NORMAL;
    sling_req.hallCallMode = sling_req.REGISTER_HALL_UNLOCK_CAR;
    sling_req.carCallMode = sling_req.AUTO_CAR;

    SlingLiftCtrl lift_ctrl(cluster_url);
    int ret = lift_ctrl.bookElevator(from_floor, to_floor, sling_req, sling_rsp);
    if (ret == 0) {
        LOGT(SLING_CALL_LIFT_MSG_HANDLER_TAG, "handle request of sling call lift OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(sling_rsp.elevatorNum);
    } else {
        LOGT(SLING_CALL_LIFT_MSG_HANDLER_TAG, "handle request of sling call lift failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling sanling interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageCallLiftRsp(rsp);
}
