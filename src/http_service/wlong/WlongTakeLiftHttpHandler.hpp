#ifndef HTTP_SERVICE_WLONG_WLONG_TAKE_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_WLONG_WLONG_TAKE_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongTakeLiftHttpHandler :  public IHttpRequestHandler {
    private:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongTakeLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_TAKE_LIFT_HTTP_HANDLER_H_