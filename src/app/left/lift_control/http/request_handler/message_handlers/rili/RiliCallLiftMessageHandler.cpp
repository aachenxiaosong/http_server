#include "RiliCallLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "UniLog.hpp"

#define RILI_CALL_LIFT_MSG_HANDLER_TAG getName().c_str()

RiliCallLiftMessageHandler :: RiliCallLiftMessageHandler() : ILiftCtrlMessageHandler("rili_call_lift_msg_handler")
{
}

RiliCallLiftMessageHandler :: ~RiliCallLiftMessageHandler()
{
}

LiftCtrlMessageRsp* RiliCallLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    #if 1
    return NULL;
    #else
    if (request.type() != MSG_LIFT_CTRL_CALL_LIFT_REQ) {
        return NULL;
    }
    const LiftCtrlMessageCallLiftReq& req = dynamic_cast<const LiftCtrlMessageCallLiftReq&>(request);
    LiftCtrlMessageCallLiftRsp rsp;
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    //step2: 根据homeId获取群控器id,并找到对应的ip和端口;并找到到达楼层
    string cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.homeId());
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    string to_floor = "";
    //step3: 根据homeId找到出发楼层
    string from_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.homeId());
    string not_found_msg = "";
    if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for home id " + req.homeId();
    }
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    }
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
    int i_cluster_id = atoi(cluster_id.c_str());
    //step4: 调用rili远程呼梯接口
    RiliResponse wl_response;
    RiliLiftCtrl rili_lift_ctrl(cluster_url, app_id, app_secret, license);
    string up_down = RILI_DN;
    if (req.upDown().compare("up")) {
        up_down = RILI_UP;
    }
    int ret = rili_lift_ctrl.bookElevator(i_cluster_id, from_floor, up_down, to_floor, req.unlockTime(), wl_response);
    if (ret == 0) {
        LOGT(RILI_CALL_LIFT_MSG_HANDLER_TAG, "handle request of rili call lift OK");
        rsp.retcode(0);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
        rsp.elevatorId(-1);
    } else {
        LOGT(RILI_CALL_LIFT_MSG_HANDLER_TAG, "handle request of rili call lift failed");
        rsp.retcode(-1);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageCallLiftRsp(rsp);
    #endif
}