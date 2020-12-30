#include "SlingCallLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SlingLiftCtrl.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"

#define SLING_CALL_LIFT_MSG_HANDLER_TAG "sling_call_lift_msg_handler"

SlingCallLiftMessageHandler :: SlingCallLiftMessageHandler() : ILiftCtrlHttpMessageHandler("sling_call_lift_handler")
{
}

SlingCallLiftMessageHandler :: ~SlingCallLiftMessageHandler()
{
}

LiftCtrlMessageRsp* SlingCallLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_CALL_LIFT_REQ) {
        return NULL;
    }
    LiftCtrlMessageCallLiftReq& req = (LiftCtrlMessageCallLiftReq &)request;
    LiftCtrlMessageCallLiftRsp rsp;
    
    return new LiftCtrlMessageCallLiftRsp(rsp);
}
