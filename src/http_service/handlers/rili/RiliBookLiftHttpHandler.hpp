#ifndef HTTP_SERVICE_HANDLERS_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_

#include "IBookLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class RiliBookLiftHttpHandler :  public IBookLiftHttpHandler {
    private:
    int handleInterVisitRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    int handleExterVisitRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    int handleGetHomeRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    RiliBookLiftHttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_