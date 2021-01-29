#include "QueryAccessibleFloorsMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "UniLog.hpp"
#include "boost/tokenizer.hpp"
#include <set>

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
    set<string> maccessible_floors;
    string accessible_floors = "";
    LiftCtrlMessageQueryAccessibleFloorsReq& req = (LiftCtrlMessageQueryAccessibleFloorsReq &)request;
    LiftCtrlMessageQueryAccessibleFloorsRsp rsp;
    LOGT(QUERY_ACCESSIBLE_FLOORS_MSG_HANDLER_TAG, "request message is handling...");
    //part1: user authorized floors
    for (auto home_id : req.authorizedHomeIds()) {
        string floor = SulinkLiftInitData :: getFloorNoBySpaceId(home_id);
        if (floor.empty()) {
            LOGE(QUERY_ACCESSIBLE_FLOORS_MSG_HANDLER_TAG, "floor number not found for home_id" + home_id);
        } else {
            // rsp.accessibleFloors().push_back(floor);
            // accessible_floors += floor;
            // accessible_floors += " ";
            maccessible_floors.insert(floor);
        }
    }
    //part2: public floors
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tok(SulinkLiftInitData :: getDevicePubFloors(req.deviceCode()), sep);
    for (auto substr :  tok) {
        // rsp.accessibleFloors().push_back(substr);
        // accessible_floors += substr;
        // accessible_floors += " ";
        maccessible_floors.insert(substr);
    }
    if (maccessible_floors.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("no accessible floor found");
        LOGE(QUERY_ACCESSIBLE_FLOORS_MSG_HANDLER_TAG, "no accessible floor found");
    } else {
        for (auto floor : maccessible_floors) {
            rsp.accessibleFloors().push_back(floor);
            accessible_floors += floor;
            accessible_floors += " ";
        }
        rsp.retcode(RETCODE_OK);
        rsp.msg("OK");
        LOGT(QUERY_ACCESSIBLE_FLOORS_MSG_HANDLER_TAG, "get accessible floor " + accessible_floors);
    }
    return new LiftCtrlMessageQueryAccessibleFloorsRsp(rsp);
}
