#include "WlongCallLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniLog.hpp"

#define WLONG_CALL_LIFT_MSG_HANDLER_TAG getName().c_str()

WlongCallLiftMessageHandler :: WlongCallLiftMessageHandler() : ILiftCtrlHttpMessageHandler("wlong_call_lift_msg_handler")
{
}

WlongCallLiftMessageHandler :: ~WlongCallLiftMessageHandler()
{
}

LiftCtrlMessageRsp* WlongCallLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
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
    } else if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    } else if (from_floor.empty()) {
        not_found_msg = "from floor not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageCallLiftRsp(rsp);
    }
    int i_cluster_id = atoi(cluster_id.c_str());
    //step4: 调用wlong远程呼梯接口
    WlongResponse wl_response;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    string up_down = WLONG_DN;
    if (req.upDown().compare("up")) {
        up_down = WLONG_UP;
    }
    int ret = wlong_lift_ctrl.bookElevator(i_cluster_id, from_floor, up_down, to_floor, req.unlockTime(), wl_response);
    if (ret == 0) {
        LOGT(WLONG_CALL_LIFT_MSG_HANDLER_TAG, "handle request of wlong call lift OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
        rsp.elevatorId(-1);
    } else {
        LOGT(WLONG_CALL_LIFT_MSG_HANDLER_TAG, "handle request of wlong call lift failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageCallLiftRsp(rsp);
}