#include "RiliBookLiftHttpHandler.hpp"
#include "uni_log.h"

#define RILI_BOOK_TAG getName().c_str()

RiliBookLiftHttpHandler :: RiliBookLiftHttpHandler() : IBookLiftHttpHandler("rili_book_lift") {
    LOGT(RILI_BOOK_TAG, "RiliBookLift object created");
}

int RiliBookLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    return 0;
}