#include "WlongCallLiftHttpHandler.hpp"
#include "log.h"

WlongCallLiftHttpHandler :: WlongCallLiftHttpHandler() : IHttpRequestHandler("wlong_call_lift") {
    LOGT("WlongCallLift object created");
}

int WlongCallLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift") != 0) {
        LOGT("%s is not for WlongCallLift", path.c_str());
        return -1;
    }
    LOGT("WlongCallLift called");
    return 0;
}