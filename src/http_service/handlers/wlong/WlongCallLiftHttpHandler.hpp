#ifndef HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_

#include "ICallLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongCallLiftHttpHandler :  public ICallLiftHttpHandler {
    private:
    int _get_home_info_from_id(CJsonObject& jinfo, int home_id, CJsonObject& jhome) ;
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongCallLiftHttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_