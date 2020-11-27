#ifndef HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT2_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT2_HTTP_HANDLER_H_

#include "IBookLift2HttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongBookLift2HttpHandler :  public IBookLift2HttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLift2HttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT2_HTTP_HANDLER_H_