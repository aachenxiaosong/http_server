#include "SlingBookLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingLiftCtrl.hpp"
#include "UniLog.hpp"

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
    LiftCtrlMessageBookLiftReq& req = (LiftCtrlMessageBookLiftReq &)request;
    LiftCtrlMessageBookLiftRsp rsp;
    
    //step1: 根据defaultHomeId获取群控器的ip和端口
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.defaultHomeId());
    string to_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.defaultHomeId());
    string not_found_msg = "";
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.defaultHomeId();
    }
    if (to_floor.empty()) {
        not_found_msg = "dest floor not found for home id " + req.defaultHomeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(-1);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    }
    if (req.authorizedHomeIds().empty() != true) {
        rsp.retcode(-1);
        rsp.msg("only sigle floor access supported");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    }
    if (req.mode().compare(req.MODE_UNLOCK) != 0) {
        rsp.retcode(-1);
        rsp.msg("only unlock mode supported");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
        return new LiftCtrlMessageBookLiftRsp(rsp);
    }
    //step2: 根据deviceCode找到出发楼层
    string from_floor;
    if (req.deviceCode().empty()) {
        from_floor = "1";
    } else {
        from_floor = SulinkLiftInitData :: getDeviceFloorNo(req.deviceCode());
    }
    //step3: 调用sling远程呼梯接口
    SlingLiftCtrl sling_lift_ctrl(cluster_url);
    int ret = sling_lift_ctrl.bookElevator(from_floor, to_floor); 
    if (ret == 0) {
        LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of sling book lift OK");
        rsp.retcode(0);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(-1);
    } else {
        LOGT(SLING_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of sling book lift failed");
        rsp.retcode(-1);
        rsp.msg("calling sanling interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftRsp(rsp);
}
