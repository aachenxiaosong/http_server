#ifndef SERVICE_WLONG_WLONG_TAKE_LIFT_HTTP_HANDLER_H_
#define SERVICE_WLONG_WLONG_TAKE_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"

using namespace std;

class WlongTakeLiftHttpHandler :  public IHttpRequestHandler {
    public:
    WlongTakeLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  SERVICE_WLONG_WLONG_TAKE_LIFT_HTTP_HANDLER_H_