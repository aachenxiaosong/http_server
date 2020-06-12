#include "WlongBookLiftHttpHandler.hpp"
#include "uni_log.h"

#define WLONG_BOOK_TAG "wlong_book"

WlongBookLiftHttpHandler :: WlongBookLiftHttpHandler() : IHttpRequestHandler("wlong_book_lift") {
    LOGT(WLONG_BOOK_TAG, "WlongBookLift object created");
}

int WlongBookLiftHttpHandler :: handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/bookLift") != 0) {
        LOGT(WLONG_BOOK_TAG, "%s is not for WlongBookLift", path.c_str());
        return -1;
    }
    LOGT(WLONG_BOOK_TAG, "WlongBookLift booked");
    return 0;
}