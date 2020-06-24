#ifndef HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_OUT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_OUT_HTTP_HANDLER_H_

#include "ICallLiftOutHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongCallLiftOutHttpHandler :  public ICallLiftOutHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongCallLiftOutHttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_OUT_HTTP_HANDLER_H_