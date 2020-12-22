#include "WlongBookLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"

#define WLONG_BOOK_LIFT_MSG_HANDLER_TAG "wlong_book_lift_msg_handler"

WlongBookLiftMessageHandler :: WlongBookLiftMessageHandler() : ILiftCtrlHttpMessageHandler("wlong_book_lift_handler")
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
    LiftCtrlMessageBookLiftReq& req = (LiftCtrlMessageBookLiftReq &)request;
    LiftCtrlMessageBookLiftRsp rsp;
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    //step2: 根据defaultHomeId获取群控器id,并找到对应的ip和端口;并找到点亮和解锁楼层
    string cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.defaultHomeId());
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.defaultHomeId());
    string open_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.defaultHomeId());
    string unlock_floors = open_floor;//for wlong, no open floor supported, so add it to unlock list
    for (int i = 0; i < req.authorizedHomeIds().size(); i++) {
        string unlock_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.authorizedHomeIds()[i]);
        if (unlock_floor.empty() != true) {
            /* if (unlock_floors.empty() != true) {
                unlock_floors += ",";
            }*/
            unlock_floors += ",";
            unlock_floors += unlock_floor;
        }
    }
    string not_found_msg = "";
    if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for home id " + req.defaultHomeId();
    }
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.defaultHomeId();
    }
    if (open_floor.empty()) {
        not_found_msg = "dest floor not found for home id " + req.defaultHomeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    }
    int i_cluster_id = atoi(cluster_id.c_str());
    //step3: 根据deviceCode找到出发楼层
    string from_floor;
    if (req.deviceCode().empty()) {
        from_floor = unisound::UniConfig::getString("liftcontrl.groundfloor");
    } else {
        from_floor = SulinkLiftInitData :: getDeviceFloorNo(req.deviceCode());
    }
    if (from_floor.empty()) {
        from_floor = "1";
    }
    //step4: 调用wlong远程呼梯接口
    WlongResponse wl_response;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    string up_down = WLONG_UP;
    int ret;
    if (req.mode().compare(req.MODE_NONE) == 0)
    {
        unlock_floors = "";
        ret = wlong_lift_ctrl.bookElevator(i_cluster_id, from_floor, up_down, unlock_floors, req.unlockTime(), wl_response);
    }
    else if (req.mode().compare(req.MODE_OPEN) == 0 ||
              (req.mode().compare(req.MODE_ALL) == 0 && req.authorizedHomeIds().size() == 0))
    {
        ret = wlong_lift_ctrl.reserveElevator(i_cluster_id, from_floor, open_floor, wl_response);
    }
    else if (req.mode().compare(req.MODE_UNLOCK) == 0 ||
               req.mode().compare(req.MODE_ALL) == 0 && req.defaultHomeId().empty() == true)
    {
        ret = wlong_lift_ctrl.bookElevator(i_cluster_id, from_floor, up_down, unlock_floors, req.unlockTime(), wl_response);
    }
    else
    {
        ret = 0;
        wl_response.code = -1;
        wl_response.msg = "mode not supported";
        wl_response.data = "";
    }   
    if (ret == 0) {
        LOGT(WLONG_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of wlong book lift OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
        rsp.elevatorId(-1);
    } else {
        LOGT(WLONG_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of wlong book lift failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftRsp(rsp);
}
