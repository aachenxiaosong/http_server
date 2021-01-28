#include "WlongTakeLiftSimpleMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniLog.hpp"
#include "boost/tokenizer.hpp"

#define WLONG_TAKE_LIFT_SIMPLE_MSG_HANDLER_TAG getName().c_str()

WlongTakeLiftSimpleMessageHandler :: WlongTakeLiftSimpleMessageHandler() : ILiftCtrlHttpMessageHandler("wlong_take_lift_simple_msg_handler")
{
}

WlongTakeLiftSimpleMessageHandler :: ~WlongTakeLiftSimpleMessageHandler()
{
}

LiftCtrlMessageRsp* WlongTakeLiftSimpleMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_TAKE_LIFT_SIMPLE_REQ) {
        return NULL;
    }
    LOGT(WLONG_TAKE_LIFT_SIMPLE_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageTakeLiftSimpleReq& req = (LiftCtrlMessageTakeLiftSimpleReq&)request;
    LiftCtrlMessageTakeLiftSimpleRsp rsp;

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
        LOGE(WLONG_TAKE_LIFT_SIMPLE_MSG_HANDLER_TAG, not_found_space_id_msg);
        return new LiftCtrlMessageTakeLiftSimpleRsp(rsp);
    }

    string unlock_floors = "";
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
        LOGE(WLONG_TAKE_LIFT_SIMPLE_MSG_HANDLER_TAG, not_found_cluster_id_msg);
        return new LiftCtrlMessageTakeLiftSimpleRsp(rsp);
    }
    int i_cluster_id = atoi(cluster_id.c_str());
	int lift_id = atoi(SulinkLiftInitData :: getDeviceLiftId(req.deviceCode()).c_str());
    

    WlongResponse wl_response;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);


    int ret = wlong_lift_ctrl.callElevatorByFoor(lift_id, open_floor, unlock_floors, wl_response);
    if (ret == 0) {
        LOGT(WLONG_TAKE_LIFT_SIMPLE_MSG_HANDLER_TAG, "handle request of wlong take lift Simple lift OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
    } else {
        LOGT(WLONG_TAKE_LIFT_SIMPLE_MSG_HANDLER_TAG, "handle request of wlong take lift Simple failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
    }
    return new LiftCtrlMessageTakeLiftSimpleRsp(rsp);
}