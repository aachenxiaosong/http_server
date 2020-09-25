#ifndef SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_TAKE_LIFT_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_TAKE_LIFT_HTTP_HANDLER_H_

#include "ITakeLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongTakeLiftHttpHandler :  public ITakeLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongTakeLiftHttpHandler();
};

#endif  //  SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_TAKE_LIFT_HTTP_HANDLER_H_