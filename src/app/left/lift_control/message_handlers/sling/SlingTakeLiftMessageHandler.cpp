#include "SlingTakeLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingLiftCtrl.hpp"
#include "UniLog.hpp"
#include "boost/tokenizer.hpp"

#define SLING_TAKE_LIFT_MSG_HANDLER_TAG getName().c_str()

SlingTakeLiftMessageHandler :: SlingTakeLiftMessageHandler() : ILiftCtrlHttpMessageHandler("sling_take_lift_msg_handler")
{
}

SlingTakeLiftMessageHandler :: ~SlingTakeLiftMessageHandler()
{
}

LiftCtrlMessageRsp* SlingTakeLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_TAKE_LIFT_REQ) {
        return NULL;
    }
    LOGT(SLING_TAKE_LIFT_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageTakeLiftReq& req = (LiftCtrlMessageTakeLiftReq&)request;
    LiftCtrlMessageTakeLiftRsp rsp;
    //step1: get cluster info through device space
    string device_space_id = SulinkLiftInitData::getDeviceSpaceId(req.deviceCode());
    string cluster_url = "";
    string cluster_id = "";
    if (device_space_id.empty() != true) {
        cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(device_space_id);
        cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(device_space_id);
    }
    //step2: 根据设备编码找到电梯id
    int elevator_id = atoi(SulinkLiftInitData :: getDeviceLiftId(req.deviceCode()).c_str());
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
        LOGE(SLING_TAKE_LIFT_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageTakeLiftRsp(rsp);
    }
    //step3: calculate open or unlock floor
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
    
    //step4: 调用sling乘梯接口
    SlingRequestAttribute sling_req;
    SlingResponse sling_rsp;
    sling_req.seqNum = rand() % 256;
    sling_req.clusterId = atoi(cluster_id.c_str());
    sling_req.elevatorId = elevator_id;
    sling_req.verificationType = sling_req.IN_CAR;
    sling_req.callAttribute = sling_req.NORMAL;
    sling_req.hallCallMode = sling_req.AUTO_HALL;
    sling_req.carCallMode = sling_req.AUTO_CAR;//change as needed

    SlingLiftCtrl lift_ctrl(cluster_url);
    int ret = -1;
    if (to_floors.size() == 1 && to_floor.floor >= 0) { //only open floor in the list, register it
        sling_req.carCallMode = sling_req.REGISTER_CAR;
        ret = lift_ctrl.bookElevator(0, to_floor, sling_req, sling_rsp);
    }   else {
        sling_req.carCallMode = sling_req.UNLOCK_CAR;
        ret = lift_ctrl.bookElevator(0, to_floors, sling_req, sling_rsp);
    }
    if (ret == 0) {
        LOGT(SLING_TAKE_LIFT_MSG_HANDLER_TAG, "handle request of sling take lift OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg("OK");
        rsp.ackCode(1);
    } else {
        LOGT(SLING_TAKE_LIFT_MSG_HANDLER_TAG, "handle request of sling take lift failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling sanling interface error");
        rsp.ackCode(0);
    }
    return new LiftCtrlMessageTakeLiftRsp(rsp);
}