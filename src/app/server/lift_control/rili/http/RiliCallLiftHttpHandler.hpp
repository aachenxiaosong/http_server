#ifndef SERVER_LIFT_CONTROL_RILI_HTTP_RILI_CALL_LIFT_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_RILI_HTTP_RILI_CALL_LIFT_HTTP_HANDLER_H_

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

#endif  //  SERVER_LIFT_CONTROL_RILI_HTTP_RILI_CALL_LIFT_HTTP_HANDLER_H_