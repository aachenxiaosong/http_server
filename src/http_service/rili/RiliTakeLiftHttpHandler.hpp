#ifndef SERVICE_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_
#define SERVICE_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"

using namespace std;

class RiliTakeLiftHttpHandler :  public IHttpRequestHandler {
    public:
    RiliTakeLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  SERVICE_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_