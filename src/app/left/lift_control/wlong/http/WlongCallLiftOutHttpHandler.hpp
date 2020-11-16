#ifndef LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_CALL_LIFT_OUT_HTTP_HANDLER_H_
#define LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_CALL_LIFT_OUT_HTTP_HANDLER_H_

#include "IBookLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;
//合并到远程呼梯的接口,特殊情况是需要写定上下行为"down"
class WlongCallLiftOutHttpHandler :  public IBookLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongCallLiftOutHttpHandler();
};

#endif  //  LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_CALL_LIFT_OUT_HTTP_HANDLER_H_