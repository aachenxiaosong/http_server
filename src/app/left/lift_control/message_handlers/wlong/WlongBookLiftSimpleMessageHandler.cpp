#include "WlongBookLiftSimpleMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"
#include "boost/tokenizer.hpp"

#define WLONG_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG "wlong_book_lift_simple_msg_handler"

WlongBookLiftSimpleMessageHandler :: WlongBookLiftSimpleMessageHandler() : ILiftCtrlHttpMessageHandler("sling_book_lift_simple_handler")
{
}

WlongBookLiftSimpleMessageHandler :: ~WlongBookLiftSimpleMessageHandler()
{
}

LiftCtrlMessageRsp* WlongBookLiftSimpleMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_SIMPLE_REQ) {
        return NULL;
    }
    LOGT(WLONG_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageBookLiftSimpleReq& req = (LiftCtrlMessageBookLiftSimpleReq &)request;
    LiftCtrlMessageBookLiftSimpleRsp rsp;
    

    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();

    string device_space_id = SulinkLiftInitData::getDeviceSpaceId(req.deviceCode());
    string cluster_url = "";
    string cluster_id = "";
    if (device_space_id.empty() == false) {
        cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(device_space_id);
        cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(device_space_id);
    }
	
    string not_found_space_id_msg = "";
    if (device_space_id.empty()) {
        not_found_space_id_msg = "space id not found for device " + req.deviceCode();
    } else if (cluster_id.empty()) {
        not_found_space_id_msg = "cluster id not found for space " + device_space_id + " of device " + req.deviceCode();
    } else if (cluster_url.empty()) {
        not_found_space_id_msg = "cluster url not found for space " + device_space_id + " of device " + req.deviceCode();
    }
    if (!not_found_space_id_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_space_id_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        LOGE(WLONG_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, not_found_space_id_msg);
        return new LiftCtrlMessageBookLiftSimpleRsp(rsp);
    }

    string open_floor = req.toFloor();
    string not_found_cluster_id_msg = "";
    if (cluster_id.empty()) {
        not_found_cluster_id_msg = "cluster id not found for deviceCode" + req.deviceCode();
    } else if (cluster_url.empty()) {
        not_found_cluster_id_msg = "cluster url not found for deviceCode" + req.deviceCode();
    } else if (open_floor.empty()) {
        not_found_cluster_id_msg = "dest floor not found for deviceCode" + req.deviceCode();
    }
    if (!not_found_cluster_id_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_cluster_id_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        LOGE(WLONG_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, not_found_cluster_id_msg);
        return new LiftCtrlMessageBookLiftSimpleRsp(rsp);
    }
    int i_cluster_id = atoi(cluster_id.c_str());
	
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
    int ret;

    ret = wlong_lift_ctrl.reserveElevator(i_cluster_id, from_floor, open_floor, wl_response);
    if (ret == 0) {
        LOGT(WLONG_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, "handle request of wlong book Simple lift OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
        rsp.elevatorId(-1);
    } else {
        LOGT(WLONG_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, "handle request of wlong book lift Simple failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftSimpleRsp(rsp);

}
