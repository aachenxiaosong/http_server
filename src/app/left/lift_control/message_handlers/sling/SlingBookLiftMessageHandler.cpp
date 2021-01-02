#include "SlingBookLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"
#include "boost/tokenizer.hpp"

#define SLING_BOOK_LIFT_MSG_HANDLER_TAG "sling_book_lift_msg_handler"

SlingBookLiftMessageHandler :: SlingBookLiftMessageHandler() : ILiftCtrlHttpMessageHandler("sling_book_lift_handler")
{
}

SlingBookLiftMessageHandler :: ~SlingBookLiftMessageHandler()
{
}

LiftCtrlMessageRsp* SlingBookLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_REQ) {
        return NULL;
    }
    LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageBookLiftReq& req = (LiftCtrlMessageBookLiftReq &)request;
    LiftCtrlMessageBookLiftRsp rsp;
    
    //step1: get cluster info through device space
    string device_space_id = SulinkLiftInitData::getDeviceSpaceId(req.deviceCode());
    string cluster_url = "";
    string cluster_id = "";
    if (req.defaultHomeId().empty() == false) {
        cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.defaultHomeId());
        cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.defaultHomeId());
    } else if (device_space_id.empty() == false) {
        cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(device_space_id);
        cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(device_space_id);
    }
    string not_found_msg = "";
    if (cluster_id.empty()) {
        if (req.defaultHomeId().empty() == false) {
            not_found_msg = "cluster id not found for home id " + req.defaultHomeId();
        } else {
            not_found_msg = "cluster id not found for space " + device_space_id + " of device " + req.deviceCode();
        }
    } else if (cluster_url.empty()) {
        if (req.defaultHomeId().empty() == false) {
            not_found_msg = "cluster url not found for home id " + req.defaultHomeId();
        } else {
            not_found_msg = "cluster url not found for space " + device_space_id + " of device " + req.deviceCode();
        }
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        LOGE(SLING_BOOK_LIFT_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    }
    SlingFloor to_floor(-1);
    vector<SlingFloor> to_floors;
    if (req.defaultHomeId().empty() != true) {
        to_floor.floor = atoi(SulinkLiftInitData :: getFloorNoBySpaceId(req.defaultHomeId()).c_str());
    }
    //part1: user authorized floors
    for (auto home_id : req.authorizedHomeIds()) {
        SlingFloor floor;
        floor.floor = atoi(SulinkLiftInitData :: getFloorNoBySpaceId(home_id).c_str());
        to_floors.push_back(floor);
    }
    //part2: public floors
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tok(SulinkLiftInitData :: getDevicePubFloors(req.deviceCode()), sep);
    for (auto substr :  tok) {
        SlingFloor floor;
        floor.floor = atoi(substr.c_str());
        to_floors.push_back(floor);
    }
    //part3: open floor
    if (to_floor.floor >= 0)
    to_floors.push_back(to_floor);
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
    //step3: 调用sling远程呼梯接口
    SlingRequestAttribute sling_req;
    SlingResponse sling_rsp;
    sling_req.seqNum = rand() % 256;
    sling_req.clusterId = atoi(cluster_id.c_str());
    sling_req.verificationLocation = req.unlockTime() > 0 ? sling_req.GATE : sling_req.LOBBY;
    sling_req.verificationType = sling_req.OUTOF_CAR;
    sling_req.callAttribute = sling_req.NORMAL;
    sling_req.hallCallMode = sling_req.AUTO_HALL;//change as needed
    sling_req.carCallMode = sling_req.AUTO_CAR;

    SlingLiftCtrl lift_ctrl(cluster_url);
    int ret = -1;
    if (req.mode().compare(req.MODE_NONE) == 0) {
        sling_req.hallCallMode = sling_req.REGISTER_HALL;
        ret = lift_ctrl.bookElevator(from_floor, to_floor, sling_req, sling_rsp);
    } else if (req.mode().compare(req.MODE_UNLOCK) == 0) {
        sling_req.hallCallMode = sling_req.REGISTER_HALL_UNLOCK_CAR;
        if (to_floors.empty()) {
            LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "unlock floors is empty");
            rsp.retcode(RETCODE_ERROR);
            rsp.msg("unlock floors is empty");
            rsp.ackCode(0);
            rsp.elevatorId(-1);
            return new LiftCtrlMessageBookLiftRsp(rsp);
        } else {
            ret = lift_ctrl.bookElevator(from_floor, to_floor, sling_req, sling_rsp);
        }
    } else if (req.mode().compare(req.MODE_OPEN) == 0) {
        sling_req.hallCallMode = sling_req.REGISTER_HALL_REGISTER_CAR;
        if (to_floor.floor < 0) {
            LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "open floor(of defaultHomeId)  is empty");
            rsp.retcode(RETCODE_ERROR);
            rsp.msg("open floors(of defaultHomeId) is empty");
            rsp.ackCode(0);
            rsp.elevatorId(-1);
            return new LiftCtrlMessageBookLiftRsp(rsp);
        } else {
            ret = lift_ctrl.bookElevator(from_floor, to_floor, sling_req, sling_rsp);
        }
    } else {
        LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "open floor(of defaultHomeId)  is empty");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("mode not supported");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    }
    if (ret == 0) {
        LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of sling book lift OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(sling_rsp.elevatorNum);
    } else {
        LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of sling book lift failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling sanling interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftRsp(rsp);
}
