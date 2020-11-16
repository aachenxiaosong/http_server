#ifndef LEFT_LIFT_CONTROL_BASE_HTTP_I_TAKE_LIFT_HTTP_HANDLER_H_
#define LEFT_LIFT_CONTROL_BASE_HTTP_I_TAKE_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class ITakeLiftHttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    void assambleResponse(int ret_code, const string& msg, int ack_code, CJsonObject& jresponse);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    ITakeLiftHttpHandler(const char* name);
    virtual ~ITakeLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  LEFT_LIFT_CONTROL_BASE_HTTP_I_TAKE_LIFT_HTTP_HANDLER_H_