#include "SlingBookLiftInterMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"

#define SLING_BOOK_LIFT_INTER_MSG_HANDLER_TAG "sling_book_lift_inter_msg_handler"

SlingBookLiftInterMessageHandler :: SlingBookLiftInterMessageHandler() : ILiftCtrlHttpMessageHandler("sling_book_lift_inter_handler")
{
}

SlingBookLiftInterMessageHandler :: ~SlingBookLiftInterMessageHandler()
{
}

LiftCtrlMessageRsp* SlingBookLiftInterMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_INTER_REQ) {
        return NULL;
    }
    LOGT(SLING_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageBookLiftInterReq& req = (LiftCtrlMessageBookLiftInterReq &)request;
    LiftCtrlMessageBookLiftInterRsp rsp;
    //step1: 根据dHomeId获取群控器的ip和端口
    string cluster_id = SulinkLiftInitData :: getClusterIdBySpaceId(req.dHomeId());
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.dHomeId());
    //step2: 根据dHomeId和sHomeId获取目的和出发楼层
    string to_floor_str = SulinkLiftInitData :: getFloorNoBySpaceId(req.dHomeId());
    string from_floor_str = SulinkLiftInitData :: getFloorNoBySpaceId(req.sHomeId());
    string not_found_msg = "";
    if (cluster_id.empty()) {
        not_found_msg = "cluster id not found for home id " + req.dHomeId();
    } else if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.dHomeId();
    } else if (to_floor_str.empty()) {
        not_found_msg = "dest floor not found for home id " + req.dHomeId();
    } else if (from_floor_str.empty()) {
        not_found_msg = "from floor not found for home id " + req.sHomeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        LOGE(SLING_BOOK_LIFT_INTER_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageBookLiftInterRsp(rsp);
    }
    SlingFloor to_floor, from_floor;
    to_floor.floor = atoi(to_floor_str.c_str());
    from_floor.floor = atoi(to_floor_str.c_str());
    //step3: 调用sling远程呼梯接口
    SlingRequestAttribute sling_req;
    SlingResponse sling_rsp;
    sling_req.seqNum = rand() % 256;
    sling_req.clusterId = atoi(cluster_id.c_str());
    sling_req.verificationLocation = sling_req.GATE;
    sling_req.verificationType = sling_req.OUTOF_CAR;
    sling_req.callAttribute = sling_req.NORMAL;
    sling_req.hallCallMode = sling_req.REGISTER_HALL_UNLOCK_CAR;
    sling_req.carCallMode = sling_req.AUTO_CAR;

    SlingLiftCtrl lift_ctrl(cluster_url);
    int ret = lift_ctrl.bookElevator(from_floor, to_floor, sling_req, sling_rsp);
    if (ret == 0) {
        LOGT(SLING_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of sling book lift inter OK");
        rsp.retcode(RETCODE_OK);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(sling_rsp.elevatorNum);
    } else {
        LOGT(SLING_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of sling book lift inter failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling sanling interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftInterRsp(rsp);
}
