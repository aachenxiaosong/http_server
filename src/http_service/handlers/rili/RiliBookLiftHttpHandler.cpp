#include "RiliBookLiftHttpHandler.hpp"
#include "InitInfo.hpp"
#include "uni_log.h"

#define RILI_BOOK_TAG getName().c_str()

RiliBookLiftHttpHandler :: RiliBookLiftHttpHandler() : IBookLiftHttpHandler("rili_book_lift") {
    LOGT(RILI_BOOK_TAG, "RiliBookLift object created");
}

int RiliBookLiftHttpHandler :: handleInterVisitRequest(CJsonObject& jrequest, CJsonObject& response) {
    int ivalue;
    string svalue;
    CJsonObject jinfo;
    InitInfo :: getInfo(jinfo);
    jrequest.Get("buildingId", ivalue);
    return 0;
}

int RiliBookLiftHttpHandler :: handleExterVisitRequest(CJsonObject& jrequest, CJsonObject& response) {
    return 0;
}

int RiliBookLiftHttpHandler :: handleGetHomeRequest(CJsonObject& jrequest, CJsonObject& response) {
    return 0;
}

int RiliBookLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    string book_type;
    jrequest.Get("bookType", book_type);
    if (book_type.compare("interVisit")) {
        handleInterVisitRequest(jrequest, jresponse);
    } else if (book_type.compare("exterVisit")) {
        handleExterVisitRequest(jrequest, jresponse);
    } else {
        handleGetHomeRequest(jrequest, jresponse);
    }
    return 0;
}