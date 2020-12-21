#include "SlingshBookLiftInterMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingshLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"

#define SLINGSH_BOOK_LIFT_INTER_MSG_HANDLER_TAG "slingsh_book_lift_inter_msg_handler"

SlingshBookLiftInterMessageHandler :: SlingshBookLiftInterMessageHandler() : ILiftCtrlHttpMessageHandler("slingsh_book_lift_inter_handler")
{
}

SlingshBookLiftInterMessageHandler :: ~SlingshBookLiftInterMessageHandler()
{
}

LiftCtrlMessageRsp* SlingshBookLiftInterMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_BOOK_LIFT_INTER_REQ) {
        return NULL;
    }
    LiftCtrlMessageBookLiftInterReq& req = (LiftCtrlMessageBookLiftInterReq &)request;
    LiftCtrlMessageBookLiftInterRsp rsp;
    
    //step1: 根据dHomeId获取群控器的ip和端口
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.dHomeId());
    //step2: 根据dHomeId和sHomeId获取目的和出发楼层
    string to_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.dHomeId());
    string from_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.sHomeId());
    string not_found_msg = "";
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
    //step3: 调用slingsh远程呼梯接口
    SlingshLiftCtrl slingsh_lift_ctrl(cluster_url);
    int ret = slingsh_lift_ctrl.bookElevator(from_floor, to_floor); 
    if (ret == 0) {
        LOGT(SLINGSH_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of slingsh book lift inter OK");
        rsp.retcode(0);
        rsp.msg("OK");
        rsp.ackCode(1);
        rsp.elevatorId(-1);
    } else {
        LOGT(SLINGSH_BOOK_LIFT_INTER_MSG_HANDLER_TAG, "handle request of slingsh book lift inter failed");
        rsp.retcode(-1);
        rsp.msg("calling sanling shanghai interface error");
        rsp.ackCode(0);
        rsp.elevatorId(-1);
    }
    return new LiftCtrlMessageBookLiftInterRsp(rsp);
}
