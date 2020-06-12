#include "WlongCallLiftHttpHandler.hpp"
#include "uni_log.h"

#define WLONG_CALL_TAG "wlong_call"

WlongCallLiftHttpHandler :: WlongCallLiftHttpHandler() : IHttpRequestHandler("wlong_call_lift") {
    LOGT(WLONG_CALL_TAG, "WlongCallLift object created");
}

int WlongCallLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift") != 0) {
        LOGT(WLONG_CALL_TAG, "%s is not for WlongCallLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_CALL_TAG, "WlongCallLift called");
    return 0;
}