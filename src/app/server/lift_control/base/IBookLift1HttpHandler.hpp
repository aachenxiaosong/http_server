#ifndef SERVER_LIFT_CONTROL_BASE_I_BOOK_LIFT1_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_BASE_I_BOOK_LIFT1_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class IBookLift1HttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    IBookLift1HttpHandler(const char* name);
    virtual ~IBookLift1HttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  SERVER_LIFT_CONTROL_BASE_I_BOOK_LIFT1_HTTP_HANDLER_H_