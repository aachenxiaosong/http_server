#ifndef SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_

#include "IBookLift1HttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongBookLift1HttpHandler :  public IBookLift1HttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLift1HttpHandler();
};

#endif  //  SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_