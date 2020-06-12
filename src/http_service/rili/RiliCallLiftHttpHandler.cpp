#include "RiliCallLiftHttpHandler.hpp"
#include "uni_log.h"

#define RILI_CALL_TAG "rili_call"

RiliCallLiftHttpHandler :: RiliCallLiftHttpHandler() : IHttpRequestHandler("rili_call_lift") {
    LOGT(RILI_CALL_TAG, "RiliCallLift object created");
}

int RiliCallLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift") != 0) {
        LOGT(RILI_CALL_TAG, "%s is not for RiliCallLift", path.c_str());
        return -1;
    }
    LOGT(RILI_CALL_TAG, "RiliCallLift called");
    return 0;
}