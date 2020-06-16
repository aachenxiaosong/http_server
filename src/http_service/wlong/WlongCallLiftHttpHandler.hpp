#ifndef HTTP_SERVICE_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongCallLiftHttpHandler :  public IHttpRequestHandler {
    private:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    int _get_home_info_from_id(CJsonObject& jinfo, int home_id, CJsonObject& jhome) ;
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongCallLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_CALL_LIFT_HTTP_HANDLER_H_