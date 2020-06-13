#ifndef HTTP_SERVICE_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"

using namespace std;

class WlongCallLiftHttpHandler :  public IHttpRequestHandler {
    public:
    WlongCallLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_