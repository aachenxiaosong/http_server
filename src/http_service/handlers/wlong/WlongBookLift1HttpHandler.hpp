#ifndef HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_
#define HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_

#include "IBookLift1HttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongBookLift1HttpHandler :  public IBookLift1HttpHandler {
    private:
    int _get_home_info_from_id(CJsonObject& jinfo, int home_id, CJsonObject& jhome) ;
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLift1HttpHandler();
};

#endif  //  HTTP_SERVICE_HANDLERS_WLONG_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_