#include "RiliCallLiftHttpHandler.hpp"
#include "uni_log.h"

#define RILI_CALL_TAG getName().c_str()

RiliCallLiftHttpHandler :: RiliCallLiftHttpHandler() : ICallLiftHttpHandler("rili_call_lift") {
    LOGT(RILI_CALL_TAG, "RiliCallLift object created");
}
int RiliCallLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    return 0;
}