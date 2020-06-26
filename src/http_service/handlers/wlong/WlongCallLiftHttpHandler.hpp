#ifndef HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_

#include "ICallLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongCallLiftHttpHandler :  public ICallLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongCallLiftHttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_