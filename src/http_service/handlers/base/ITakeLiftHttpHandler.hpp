#ifndef HTTP_SERVICE_HANDLERS_BASE_I_TAKE_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_BASE_I_TAKE_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class ITakeLiftHttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    ITakeLiftHttpHandler(const char* name);
    virtual ~ITakeLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_HANDLERS_BASE_I_TAKE_LIFT_HTTP_HANDLER_H_