#ifndef HTTP_SERVICE_HANDLERS_BASE_I_BOOK_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_BASE_I_BOOK_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class IBookLiftHttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    IBookLiftHttpHandler(const char* name);
    virtual ~IBookLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_HANDLERS_BASE_I_BOOK_LIFT_HTTP_HANDLER_H_