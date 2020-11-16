#ifndef LEFT_LIFT_CONTROL_BASE_HTTP_I_LIFT_STATUS_HTTP_HANDLER_H_
#define LEFT_LIFT_CONTROL_BASE_HTTP_I_LIFT_STATUS_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class ILiftStatusHttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    void assambleResponse(int ret_code, const string& msg, const string& cur_floor, const string& direction, const string& moving_status, const string& door_status, CJsonObject& jresponse);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    ILiftStatusHttpHandler(const char* name);
    virtual ~ILiftStatusHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  LEFT_LIFT_CONTROL_BASE_HTTP_I_LIFT_STATUS_HTTP_HANDLER_H_
