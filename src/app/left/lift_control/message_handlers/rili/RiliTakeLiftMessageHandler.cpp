#include "RiliTakeLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "UniLog.hpp"

#define RILI_TAKE_LIFT_MSG_HANDLER_TAG getName().c_str()

RiliTakeLiftMessageHandler :: RiliTakeLiftMessageHandler() : ILiftCtrlHttpMessageHandler("rili_take_lift_msg_handler")
{
}

RiliTakeLiftMessageHandler :: ~RiliTakeLiftMessageHandler()
{
}

LiftCtrlMessageRsp* RiliTakeLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_TAKE_LIFT_REQ) {
        return NULL;
    }
    LiftCtrlMessageTakeLiftRsp rsp;
    LOGT(RILI_TAKE_LIFT_MSG_HANDLER_TAG, "request of Rili interface(take lift) not supported");
    rsp.retcode(-1);
    rsp.msg("request of Rili interface(take lift) not supported");
    rsp.ackCode(0);
    return new LiftCtrlMessageTakeLiftRsp(rsp);
}