#include "QueryAccessibleFloorsMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "UniLog.hpp"
#include "boost/tokenizer.hpp"

#define QUERY_ACCESSIBLE_FLOORS_MSG_HANDLER_TAG getName().c_str()

QueryAccessibleFloorsMessageHandler :: QueryAccessibleFloorsMessageHandler() : ILiftCtrlHttpMessageHandler("query_accessible_floors_msg_handler")
{
}

QueryAccessibleFloorsMessageHandler :: ~QueryAccessibleFloorsMessageHandler()
{
}

LiftCtrlMessageRsp* QueryAccessibleFloorsMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_QUERY_ACCESSIBLE_FLOORS_REQ) {
        return NULL;
    }
    LiftCtrlMessageQueryAccessibleFloorsReq& req = (LiftCtrlMessageQueryAccessibleFloorsReq &)request;
    LiftCtrlMessageQueryAccessibleFloorsRsp rsp;
    LOGT(QUERY_ACCESSIBLE_FLOORS_MSG_HANDLER_TAG, "request message is handling...");
    //part1: user authorized floors
    for (auto home_id : req.authorizedHomeIds()) {
        rsp.accessibleFloors().push_back(SulinkLiftInitData :: getFloorNoBySpaceId(home_id));
    }
    //part2: public floors
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tok(SulinkLiftInitData :: getDevicePubFloors(req.deviceCode()), sep);
    for (auto substr :  tok) {
        rsp.accessibleFloors().push_back(substr);
    }
    if (rsp.accessibleFloors().empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("no accessible floor found");
    } else {
        rsp.retcode(RETCODE_OK);
        rsp.msg("OK");
    }
    return new LiftCtrlMessageQueryAccessibleFloorsRsp(rsp);
}