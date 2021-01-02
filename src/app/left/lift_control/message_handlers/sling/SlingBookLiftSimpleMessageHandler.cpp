#include "SlingBookLiftSimpleMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"
#include "boost/tokenizer.hpp"

#define SLING_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG "sling_book_lift_simple_msg_handler"

SlingBookLiftSimpleMessageHandler :: SlingBookLiftSimpleMessageHandler() : ILiftCtrlHttpMessageHandler("sling_book_lift_simple_handler")
{
}

SlingBookLiftSimpleMessageHandler :: ~SlingBookLiftSimpleMessageHandler()
{
}

LiftCtrlMessageRsp* SlingBookLiftSimpleMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_SIMPLE_REQ) {
        return NULL;
    }
    LOGT(SLING_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageBookLiftSimpleReq& req = (LiftCtrlMessageBookLiftSimpleReq &)request;
    LiftCtrlMessageBookLiftSimpleRsp rsp;
    
    //step1: get cluster info through device space
    string device_space_id = SulinkLiftInitData::getDeviceSpaceId(req.deviceCode());
    string cluster_url = "";
    string cluster_id = "";
    if (device_space_id.empty() != true) {
        cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(device_space_id);
        cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(device_space_id);
    }
    string not_found_msg = "";
    if (device_space_id.empty()) {
        not_found_msg = "space id not found for device " + req.deviceCode();
    } else if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for space " + device_space_id + " of device " + req.deviceCode();
    } else if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for space " + device_space_id + " of device " + req.deviceCode();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        LOGE(SLING_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageBookLiftSimpleRsp(rsp);
    }
    //step2: 根据deviceCode找到出发楼层
    string from_floor_str;
    if (req.deviceCode().empty()) {
        from_floor_str = unisound::UniConfig::getString("liftcontrl.groundfloor");
    } else {
        from_floor_str = SulinkLiftInitData :: getDeviceFloorNo(req.deviceCode());
    }
    if (from_floor_str.empty()) {
        from_floor_str = "1";
    }
    SlingFloor from_floor = atoi(from_floor_str.c_str());
    //step3: to floor
    SlingFloor to_floor(-1);
    to_floor.floor = atoi(req.toFloor().c_str());
    //step4: 调用sling远程呼梯接口
    SlingRequestAttribute sling_req;
    SlingResponse sling_rsp;
    sling_req.seqNum = rand() % 256;
    sling_req.clusterId = atoi(cluster_id.c_str());
    sling_req.verificationLocation = sling_req.LOBBY;
    sling_req.verificationType = sling_req.OUTOF_CAR;
    sling_req.callAttribute = sling_req.NORMAL;
    sling_req.hallCallMode = sling_req.AUTO_HALL;//change as needed
    sling_req.carCallMode = sling_req.AUTO_CAR;

    SlingLiftCtrl lift_ctrl(cluster_url);
    int ret = -1;
    sling_req.hallCallMode = sling_req.REGISTER_HALL_REGISTER_CAR;
    ret = lift_ctrl.bookElevator(from_floor, to_floor, sling_req, sling_rsp);
    if (ret == 0) {
        LOGT(SLING_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, "handle request of sling book lift simple OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(sling_rsp.elevatorNum);
    } else {
        LOGT(SLING_BOOK_LIFT_SIMPLE_MSG_HANDLER_TAG, "handle request of sling book lift simple failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling sanling interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftSimpleRsp(rsp);
}
