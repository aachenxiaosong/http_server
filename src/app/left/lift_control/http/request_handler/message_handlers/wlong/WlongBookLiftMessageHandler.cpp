#include "WlongBookLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "uni_log.h"

#define WLONG_BOOK_LIFT_MSG_HANDLER_TAG "wlong_book_lift_msg_handler"

WlongBookLiftMessageHandler :: WlongBookLiftMessageHandler() : ILiftCtrlMessageHandler("wlong_book_lift_handler")
{
}

WlongBookLiftMessageHandler :: ~WlongBookLiftMessageHandler()
{
}

LiftCtrlMessageRsp* WlongBookLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_REQ) {
        return NULL;
    }
    const LiftCtrlMessageBookLiftReq& req = dynamic_cast<const LiftCtrlMessageBookLiftReq&>(request);
    LiftCtrlMessageBookLiftRsp rsp;
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    //step2: 根据homeId获取群控器id,并找到对应的ip和端口;并找到到达楼层
    string cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.homeId());
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.homeId());
    string to_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.homeId());
    string not_found_msg = "";
    if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for home id " + req.homeId();
    }
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.homeId();
    }
    if (to_floor.empty()) {
        not_found_msg = "dest floor not found for home id " + req.homeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    }
    int i_cluster_id = atoi(cluster_id.c_str());
    //step3: 根据deviceCode找到出发楼层
    string from_floor;
    if (req.deviceCode().empty()) {
        from_floor = "1";
    } else {
        from_floor = SulinkLiftInitData :: getDeviceFloorNo(req.deviceCode());
    }
    //step4: 调用wlong远程呼梯接口
    WlongResponse wl_response;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    string up_down = WLONG_UP;
    int ret = wlong_lift_ctrl.bookingElevator(i_cluster_id, from_floor, up_down, to_floor, req.unlockTime(), wl_response);
    if (ret == 0) {
        LOGT(WLONG_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of wlong book lift OK");
        rsp.retcode(0);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
        rsp.elevatorId(-1);
    } else {
        LOGT(WLONG_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of wlong book lift failed");
        rsp.retcode(-1);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftRsp(rsp);
}