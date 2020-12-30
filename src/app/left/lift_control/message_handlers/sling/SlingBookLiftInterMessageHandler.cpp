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
    LiftCtrlMessageBookLiftInterReq& req = (LiftCtrlMessageBookLiftInterReq &)request;
    LiftCtrlMessageBookLiftInterRsp rsp;
    
    return new LiftCtrlMessageBookLiftInterRsp(rsp);
}
