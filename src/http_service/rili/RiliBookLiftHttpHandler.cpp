#include "RiliBookLiftHttpHandler.hpp"
#include "uni_log.h"

#define RILI_BOOK_TAG "rili_book"

RiliBookLiftHttpHandler :: RiliBookLiftHttpHandler() : IHttpRequestHandler("rili_book_lift") {
    LOGT(RILI_BOOK_TAG, "RiliBookLift object created");
}

int RiliBookLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift") != 0) {
        LOGT(RILI_BOOK_TAG, "%s is not for RiliBookLift", path.c_str());
        return -1;
    }
    LOGT(RILI_BOOK_TAG, "RiliBookLift booked");
    return 0;
}