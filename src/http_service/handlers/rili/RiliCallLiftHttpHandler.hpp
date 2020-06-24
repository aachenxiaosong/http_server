#ifndef HTTP_SERVICE_HANDLERS_RILI_RILI_CALL_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_RILI_RILI_CALL_LIFT_HTTP_HANDLER_H_

#include "ICallLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class RiliCallLiftHttpHandler :  public ICallLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    RiliCallLiftHttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_RILI_RILI_CALL_LIFT_HTTP_HANDLER_H_