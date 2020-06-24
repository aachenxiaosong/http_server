#ifndef HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT_HTTP_HANDLER_H_

#include "IBookLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongBookLiftHttpHandler :  public IBookLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLiftHttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT_HTTP_HANDLER_H_