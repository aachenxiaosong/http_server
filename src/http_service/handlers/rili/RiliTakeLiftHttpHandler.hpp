#ifndef HTTP_SERVICE_HANDLERS_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_

#include "ITakeLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class RiliTakeLiftHttpHandler :  public ITakeLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    RiliTakeLiftHttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_