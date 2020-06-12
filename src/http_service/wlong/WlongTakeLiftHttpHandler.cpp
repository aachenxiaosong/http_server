#include "WlongTakeLiftHttpHandler.hpp"
#include "uni_log.h"

#define WLONG_TAKE_TAG "wlong_take"

WlongTakeLiftHttpHandler :: WlongTakeLiftHttpHandler() : IHttpRequestHandler("wlong_take_lift") {
    LOGT(WLONG_TAKE_TAG, "WlongTakeLift object created");
}

int WlongTakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/takeLift") != 0) {
        LOGT(WLONG_TAKE_TAG, "%s is not for WlongTakeLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_TAKE_TAG, "WlongTakeLift takeed");
    return 0;
}