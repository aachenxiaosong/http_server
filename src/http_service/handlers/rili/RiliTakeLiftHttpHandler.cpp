#include "RiliTakeLiftHttpHandler.hpp"
#include "uni_log.h"

#define RILI_TAKE_TAG getName().c_str()

RiliTakeLiftHttpHandler :: RiliTakeLiftHttpHandler() : ITakeLiftHttpHandler("rili_take_lift") {
    LOGT(RILI_TAKE_TAG, "RiliTakeLift object created");
}

int RiliTakeLiftHttpHandler :: handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) {
    return 0;
}