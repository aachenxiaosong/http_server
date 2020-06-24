#ifndef HTTP_SERVICE_HANDLERS_BASE_I_CALL_LIFT_OUT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_BASE_I_CALL_LIFT_OUT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class ICallLiftOutHttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    ICallLiftOutHttpHandler(const char* name);
    virtual ~ICallLiftOutHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_HANDLERS_BASE_I_CALL_LIFT_OUT_HTTP_HANDLER_H_
