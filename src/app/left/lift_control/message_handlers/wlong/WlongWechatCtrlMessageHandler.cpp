#include "WlongWechatCtrlMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniLog.hpp"

#define WLONG_WECHAT_CTRL_MSG_HANDLER_TAG getName().c_str()

WlongWechatCtrlMessageHandler :: WlongWechatCtrlMessageHandler() : ILiftCtrlMqMessageHandler("wlong_wechat_ctrl_msg_handler")
{
}

WlongWechatCtrlMessageHandler :: ~WlongWechatCtrlMessageHandler()
{
}

int WlongWechatCtrlMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_WECHAT_CTRL) {
        return -1;
    }
    LOGT(WLONG_WECHAT_CTRL_MSG_HANDLER_TAG, "request message is handling...");
    const LiftCtrlMessageWechatCtrl& req = dynamic_cast<const LiftCtrlMessageWechatCtrl&>(request);
    LiftCtrlMessageWechatCtrlAck rsp;
    rsp.reqId(req.reqId());
    if (req.retcode() != 0) {
        rsp.retcode(req.retcode());
        rsp.msg(req.msg());
        //send ack
        string *content = mPacker.pack(rsp);
        MqData data(MQ_TOPIC_SULINK_LIFT_CTRL, (void *)content->c_str(), content->length() + 1);
        mMq.send(data);
        return 0;
    }
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    //step2: 根据homeId获取群控器id,并找到对应的ip和端口
    string cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.homeId());
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    //step3: 根据homeId找到出发楼层
    string from_floor = req.fromFloor();
    string to_floor = req.toFloor();
    string not_found_msg = "";
    if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for home id " + req.homeId();
    } else if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    }

    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        LOGE(WLONG_WECHAT_CTRL_MSG_HANDLER_TAG, not_found_msg);
        //send ack
        string *content = mPacker.pack(rsp);
        MqData data(MQ_TOPIC_SULINK_LIFT_CTRL, (void *)content->c_str(), content->length() + 1);
        mMq.send(data);
        return 0;
    }
    int i_cluster_id = atoi(cluster_id.c_str());
    //step4: 调用wlong远程呼梯接口
    WlongResponse wl_response;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    string up_down = WLONG_UP;
    if (from_floor.compare(to_floor) > 0) {
        up_down = WLONG_DN;//down
    }
    int ret = wlong_lift_ctrl.bookElevator(i_cluster_id, from_floor, up_down, to_floor, SulinkLiftInitData::getUnlockTime(), wl_response);
    if (ret == 0) {
        //different treatment for wechat
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            LOGT(WLONG_WECHAT_CTRL_MSG_HANDLER_TAG, "calling WangLong book lift interface succeed");
            rsp.retcode(0);
            //get status, TODO: elevator Id is hard coded as 1
            rsp.elevatorId("1");
            WlongLiftStatus status;
            ret = wlong_lift_ctrl.getElevatorStatus(atoi(rsp.elevatorId().c_str()), status);
            if (ret == 0) {
                if (status.code == 0)
                {
                    rsp.curFloor(to_string(status.cur_floor));
                    if (status.upward)
                    {
                        rsp.upDown("up");
                    }
                    else
                    {
                        rsp.upDown("down");
                    }
                } else {
                    rsp.msg("get lift status failed for calling Wanglong interface rejected for " + status.msg);
                    LOGE(WLONG_WECHAT_CTRL_MSG_HANDLER_TAG, "get lift status failed for calling Wanglong interface rejected for " + status.msg);
                }
            } else {
                //booking lift ok, but geting status fialed => still set retcode as 0
                rsp.msg("get lift status failed for calling WangLong interface failed");
                LOGE(WLONG_WECHAT_CTRL_MSG_HANDLER_TAG, "get lift status failed for calling WangLong interface failed");
            }
        } else {
            LOGT(WLONG_WECHAT_CTRL_MSG_HANDLER_TAG, "calling WangLong book lift interface rejected for " + wl_response.msg);
            rsp.retcode(1);
            rsp.msg(rsp.msg() + "calling WangLong book lift interface rejected for " + wl_response.msg);
        }
    } else {
        rsp.retcode(1);
        rsp.msg("calling WangLong book lift interface failed");
        LOGT(WLONG_WECHAT_CTRL_MSG_HANDLER_TAG, "handle request of wlong book lift failed");
    }
    //send ack
    string *content = mPacker.pack(rsp);
    MqData data(MQ_TOPIC_SULINK_LIFT_CTRL, (void *)content->c_str(), content->length() + 1);
    mMq.send(data);
    return 0;
}
