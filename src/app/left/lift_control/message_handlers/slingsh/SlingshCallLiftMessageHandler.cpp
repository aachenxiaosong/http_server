#include "SlingshCallLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingshLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"

#define SLINGSH_CALL_LIFT_MSG_HANDLER_TAG "slingsh_call_lift_msg_handler"

SlingshCallLiftMessageHandler :: SlingshCallLiftMessageHandler() : ILiftCtrlHttpMessageHandler("slingsh_call_lift_handler")
{
}

SlingshCallLiftMessageHandler :: ~SlingshCallLiftMessageHandler()
{
}

LiftCtrlMessageRsp* SlingshCallLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_CALL_LIFT_REQ) {
        return NULL;
    }
    LiftCtrlMessageCallLiftReq& req = (LiftCtrlMessageCallLiftReq &)request;
    LiftCtrlMessageCallLiftRsp rsp;
    //step1: 根据homeId获取群控器的ip和端口
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    string not_found_msg = "";
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    }
    //step2: 根据homeId找到出发楼层，到达楼层设置为1
    string from_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.homeId());
    if (from_floor.empty()) {
        not_found_msg = "from floor not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageCallLiftRsp(rsp);
    }
    //TODO: 只支持到达1楼，updown参数无效了
    string to_floor = unisound::UniConfig::getString("liftcontrl.groundfloor");
    if (to_floor.empty()) {
        to_floor = "1";
    }  
    //step3: 调用slingsh远程呼梯接口
    SlingshLiftCtrl slingsh_lift_ctrl(cluster_url);
    int ret = slingsh_lift_ctrl.bookElevator(from_floor, to_floor); 
    if (ret == 0) {
        LOGT(SLINGSH_CALL_LIFT_MSG_HANDLER_TAG, "handle request of slingsh call lift OK");
        rsp.retcode(0);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(-1);
    } else {
        LOGT(SLINGSH_CALL_LIFT_MSG_HANDLER_TAG, "handle request of slingsh call lift failed");
        rsp.retcode(-1);
        rsp.msg("calling sanling shanghai interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageCallLiftRsp(rsp);
}
