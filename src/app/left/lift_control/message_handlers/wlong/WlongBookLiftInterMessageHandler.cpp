#include "WlongBookLiftInterMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniLog.hpp"

#define WLONG_BOOK_LIFT_INTER_MSG_HANDLER_TAG "wlong_book_lift_inter_msg_handler"

WlongBookLiftInterMessageHandler :: WlongBookLiftInterMessageHandler() : ILiftCtrlMessageHandler("wlong_book_lift_inter_handler")
{
}

WlongBookLiftInterMessageHandler :: ~WlongBookLiftInterMessageHandler()
{
}

LiftCtrlMessageRsp* WlongBookLiftInterMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_INTER_REQ) {
        return NULL;
    }
    const LiftCtrlMessageBookLiftInterReq& req = dynamic_cast<const LiftCtrlMessageBookLiftInterReq&>(request);
    LiftCtrlMessageBookLiftInterRsp rsp;
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    //step2: 根据dHomeId获取群控器id,并找到对应的ip和端口;并找到到达楼层
    string cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.dHomeId());
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.dHomeId());
    string to_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.dHomeId());
    //step3: 根据sHomeId找到出发楼层
    string from_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.sHomeId());
    string not_found_msg = "";
    if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for home id " + req.dHomeId();
    }
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.dHomeId();
    }
    if (to_floor.empty()) {
        not_found_msg = "dest floor not found for home id " + req.dHomeId();
    }
    if (from_floor.empty()) {
        not_found_msg = "from floor not found for home id " + req.sHomeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftInterRsp(rsp);
    }
    int i_cluster_id = atoi(cluster_id.c_str());
    //step4: 调用wlong远程呼梯接口
    WlongResponse wl_response;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    string up_down = WLONG_UP;
    if (from_floor.compare(to_floor) > 0) {
        up_down = WLONG_DN;//down
    }
    int ret = wlong_lift_ctrl.bookElevator(i_cluster_id, from_floor, up_down, to_floor, req.unlockTime(), wl_response);
    if (ret == 0) {
        LOGT(WLONG_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of wlong book lift internal OK");
        rsp.retcode(0);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
        rsp.elevatorId(-1);
    } else {
        LOGT(WLONG_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of wlong book lift internal failed");
        rsp.retcode(-1);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftInterRsp(rsp);
}