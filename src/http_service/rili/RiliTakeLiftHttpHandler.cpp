#include "RiliTakeLiftHttpHandler.hpp"
#include "uni_log.h"

#define RILI_TAKE_TAG "rili_take"

RiliTakeLiftHttpHandler :: RiliTakeLiftHttpHandler() : IHttpRequestHandler("rili_take_lift") {
    LOGT(RILI_TAKE_TAG, "RiliTakeLift object created");
}

int RiliTakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/takeLift") != 0) {
        LOGT(RILI_TAKE_TAG, "%s is not for RiliTakeLift", path.c_str());
        return -1;
    }
    LOGT(RILI_TAKE_TAG, "RiliTakeLift takeed");
    return 0;
}