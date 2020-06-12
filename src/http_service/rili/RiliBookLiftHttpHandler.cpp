#include "RiliBookLiftHttpHandler.hpp"
#include "log.h"

RiliBookLiftHttpHandler :: RiliBookLiftHttpHandler() : IHttpRequestHandler("rili_book_lift") {
    LOGT("RiliBookLift object created");
}

int RiliBookLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift") != 0) {
        LOGT("%s is not for RiliBookLift", path.c_str());
        return -1;
    }
    LOGT("RiliBookLift booked");
    return 0;
}