#ifndef SERVICE_RILI_RILI_CALL_LIFT_H_
#define SERVICE_RILI_RILI_CALL_LIFT_H_

#include "http_request_handler.h"

using namespace std;

class RiliCallLift :  public HttpRequestHandler {
    public:
    RiliCallLift();
    int handle(string& path, string& request, string&response) ;
};

#endif