#ifndef LEFT_LIFT_CONTROL_BASE_HTTP_I_BOOK_LIFT_INTER_HTTP_HANDLER_H_
#define LEFT_LIFT_CONTROL_BASE_HTTP_I_BOOK_LIFT_INTER_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class IBookLiftInterHttpHandler :  public IHttpRequestHandler {
    protected:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    void assambleResponse(int ret_code, const string& msg, int ack_code, int elevator_id, CJsonObject& jresponse);
    virtual int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse) = 0;
    public:
    IBookLiftInterHttpHandler(const char* name);
    virtual ~IBookLiftInterHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  LEFT_LIFT_CONTROL_BASE_HTTP_I_BOOK_LIFT_INTER_HTTP_HANDLER_H_