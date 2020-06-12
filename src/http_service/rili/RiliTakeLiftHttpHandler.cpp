#include "RiliTakeLiftHttpHandler.hpp"
#include "log.h"

RiliTakeLiftHttpHandler :: RiliTakeLiftHttpHandler() : IHttpRequestHandler("rili_take_lift") {
    LOGT("RiliTakeLift object created");
}

int RiliTakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/takeLift") != 0) {
        LOGT("%s is not for RiliTakeLift", path.c_str());
        return -1;
    }
    LOGT("RiliTakeLift takeed");
    return 0;
}