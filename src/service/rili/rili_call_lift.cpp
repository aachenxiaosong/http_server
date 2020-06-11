#include "rili_call_lift.h"
#include "log.h"

RiliCallLift::RiliCallLift() : HttpRequestHandler("rili_call_lift") {
    LOGT("RiliCallLift object created");
}

int RiliCallLift::handle(string& path, string& request, string& response) {
    if (path.compare("/liftCtrl/v2/callLift") != 0) {
        LOGT("%s is not for RiliCallLift", path.c_str());
        return -1;
    }
    LOGT("RiliCallLift called");
    return 0;
}