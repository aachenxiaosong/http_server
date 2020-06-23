#ifndef HTTP_SERVICE_WLONG_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_
#define HTTP_SERVICE_WLONG_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongBookLift1HttpHandler :  public IHttpRequestHandler {
    private:
    int checkRqeust(CJsonObject& jrequest, string& err_field);
    int _get_home_info_from_id(CJsonObject& jinfo, int home_id, CJsonObject& jhome) ;
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLift1HttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_