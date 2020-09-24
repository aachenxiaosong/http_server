#ifndef SERVER_LIFT_CONTROL_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_

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

#endif  //  SERVER_LIFT_CONTROL_RILI_RILI_TAKE_LIFT_HTTP_HANDLER_H_