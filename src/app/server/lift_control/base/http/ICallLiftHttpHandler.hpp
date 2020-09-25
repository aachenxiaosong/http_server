#ifndef SERVER_LIFT_CONTROL_BASE_HTTP_I_CALL_LIFT_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_BASE_HTTP_I_CALL_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class ICallLiftHttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    ICallLiftHttpHandler(const char* name);
    virtual ~ICallLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  SERVER_LIFT_CONTROL_BASE_HTTP_I_CALL_LIFT_HTTP_HANDLER_H_