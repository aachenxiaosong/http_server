#include "WlongWechatStatusMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniLog.hpp"

#define WLONG_WECHAT_STATUS_MSG_HANDLER_TAG getName().c_str()

WlongWechatStatusMessageHandler :: WlongWechatStatusMessageHandler() : ILiftCtrlMqMessageHandler("wlong_wechat_status_msg_handler")
{
}

WlongWechatStatusMessageHandler :: ~WlongWechatStatusMessageHandler()
{
}

int WlongWechatStatusMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_WECHAT_STATUS) {
        return -1;
    }
    LOGT(WLONG_WECHAT_STATUS_MSG_HANDLER_TAG, "request is being handled");
    const LiftCtrlMessageWechatStatus& req = dynamic_cast<const LiftCtrlMessageWechatStatus&>(request);
    LiftCtrlMessageWechatStatusAck rsp;
    rsp.reqId(req.reqId());
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    //step2: 根据homeId获取群控器ip和端口
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    //step3: 根据homeId找到出发楼层
    int elevator_id = atoi(req.elevatorId().c_str());
    string not_found_msg = "";
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        LOGE(WLONG_WECHAT_STATUS_MSG_HANDLER_TAG, "%s", not_found_msg.c_str());
        //send ack
        string *content = mPacker.pack(rsp);
        MqData data(MQ_TOPIC_SULINK_LIFT_CTRL, (void *)content->c_str(), content->length() + 1);
        mMq.send(data);
        return 0;
    }
    //step4: 调用wlong远程呼梯接口
    WlongLiftStatus wl_lift_status;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    int ret = wlong_lift_ctrl.getElevatorStatus(elevator_id, wl_lift_status);
    if (ret == 0) {
        LOGT(WLONG_WECHAT_STATUS_MSG_HANDLER_TAG, "calling WangLong lift satus interface succeed");
        rsp.msg(wl_lift_status.msg);
        if (wl_lift_status.code == 0) {
            rsp.retcode(0);
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
        rsp.retcode(1);
        rsp.msg("calling WangLong lift status interface failed");
        LOGT(WLONG_WECHAT_STATUS_MSG_HANDLER_TAG, "handle request of wlong lift status failed");
    }
    //send ack
    string *content = mPacker.pack(rsp);
    MqData data(MQ_TOPIC_SULINK_LIFT_CTRL, (void *)content->c_str(), content->length() + 1);
    mMq.send(data);
    return 0;
}
