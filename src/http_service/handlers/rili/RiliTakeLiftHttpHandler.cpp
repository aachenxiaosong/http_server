#include "RiliTakeLiftHttpHandler.hpp"
#include "uni_log.h"

#define RILI_TAKE_TAG getName().c_str()

RiliTakeLiftHttpHandler :: RiliTakeLiftHttpHandler() : ITakeLiftHttpHandler("rili_take_lift") {
    LOGT(RILI_TAKE_TAG, "RiliTakeLift object created");
}

int RiliTakeLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    jresponse.Add("errCode", 1);
    jresponse.Add("errMsg", "request of Rili 3p interface not supported");
    jresponse.Add("ackCode", 0);
    jresponse.Add("elevatorId", -1);
    return 0;
}