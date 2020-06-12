#include "WlongTakeLiftHttpHandler.hpp"
#include "log.h"

WlongTakeLiftHttpHandler :: WlongTakeLiftHttpHandler() : IHttpRequestHandler("wlong_take_lift") {
    LOGT("WlongTakeLift object created");
}

int WlongTakeLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/takeLift") != 0) {
        LOGT("%s is not for WlongTakeLift", path.c_str());
        return -1;
    }
    LOGT("WlongTakeLift takeed");
    return 0;
}