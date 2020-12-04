#include "WlongLiftStatusMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniLog.hpp"

#define WLONG_LIFT_STATUS_MSG_HANDLER_TAG getName().c_str()

WlongLiftStatusMessageHandler :: WlongLiftStatusMessageHandler() : ILiftCtrlMessageHandler("wlong_lift_status_msg_handler")
{
}

WlongLiftStatusMessageHandler :: ~WlongLiftStatusMessageHandler()
{
}

LiftCtrlMessageRsp* WlongLiftStatusMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_LIFT_STATUS_REQ) {
        return NULL;
    }
    LiftCtrlMessageLiftStatusReq& req = (LiftCtrlMessageLiftStatusReq&)request;
    LiftCtrlMessageLiftStatusRsp rsp;
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    string not_found_msg = "";
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        return new LiftCtrlMessageLiftStatusRsp(rsp);
    }
    //step2: 调用wlong乘梯接口
    WlongLiftStatus wl_lift_status;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    int ret = wlong_lift_ctrl.getElevatorStatus(req.elevatorId(), wl_lift_status);
    if (ret == 0) {
        LOGT(WLONG_LIFT_STATUS_MSG_HANDLER_TAG, "handle request of wlong lift status OK");
        rsp.retcode(0);
        rsp.msg(wl_lift_status.msg);
        if (wl_lift_status.code == 0) {
            rsp.curFloor(to_string(wl_lift_status.cur_floor));
            if (wl_lift_status.upward) {
                rsp.direction("up");
            } else {
                rsp.direction("down");
            }
            if (wl_lift_status.status_error) {
                rsp.movingStatus("error");
            } else if (wl_lift_status.stopped) {
                rsp.movingStatus("stopped");
            } else {
                rsp.movingStatus("moving");
            }
            if (wl_lift_status.open) {
                rsp.doorStatus("open");
            } else if (wl_lift_status.closed) {
                rsp.doorStatus("closed");
            } else if (wl_lift_status.opening) {
                rsp.doorStatus("opening");
            } else if (wl_lift_status.closing) {
                rsp.doorStatus("closing");
            } else {
                rsp.doorStatus("error");
            }
        }
    } else {
        LOGT(WLONG_LIFT_STATUS_MSG_HANDLER_TAG, "handle request of wlong lift status failed");
        rsp.retcode(-1);
        rsp.msg("calling WangLong interface error");
    }
    return new LiftCtrlMessageLiftStatusRsp(rsp);
}