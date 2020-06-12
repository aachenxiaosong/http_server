#include "WlongBookLiftHttpHandler.hpp"
#include "log.h"

WlongBookLiftHttpHandler :: WlongBookLiftHttpHandler() : IHttpRequestHandler("wlong_book_lift") {
    LOGT("WlongBookLift object created");
}

int WlongBookLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift") != 0) {
        LOGT("%s is not for WlongBookLift", path.c_str());
        return -1;
    }
    LOGT("WlongBookLift booked");
    return 0;
}