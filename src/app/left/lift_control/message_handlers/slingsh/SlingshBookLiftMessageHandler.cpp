#include "SlingshBookLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingshLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"

#define SLINGSH_BOOK_LIFT_MSG_HANDLER_TAG "slingsh_book_lift_msg_handler"

SlingshBookLiftMessageHandler :: SlingshBookLiftMessageHandler() : ILiftCtrlHttpMessageHandler("slingsh_book_lift_handler")
{
}

SlingshBookLiftMessageHandler :: ~SlingshBookLiftMessageHandler()
{
}

LiftCtrlMessageRsp* SlingshBookLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
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
        from_floor = unisound::UniConfig::getString("liftcontrl.groundfloor");
    } else {
        from_floor = SulinkLiftInitData :: getDeviceFloorNo(req.deviceCode());
    }
    if (from_floor.empty()) {
        from_floor = "1";
    }
    //step3: 调用slingsh远程呼梯接口
    SlingshLiftCtrl slingsh_lift_ctrl(cluster_url);
    int ret = slingsh_lift_ctrl.bookElevator(from_floor, to_floor); 
    if (ret == 0) {
        LOGT(SLINGSH_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of slingsh book lift OK");
        rsp.retcode(0);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(-1);
    } else {
        LOGT(SLINGSH_BOOK_LIFT_MSG_HANDLER_TAG, "handle request of slingsh book lift failed");
        rsp.retcode(-1);
        rsp.msg("calling sanling shanghai interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftRsp(rsp);
}
