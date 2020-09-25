#ifndef SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT_HTTP_HANDLER_H_

#include "IBookLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongBookLiftHttpHandler :  public IBookLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLiftHttpHandler();
};

#endif  //  SERVER_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT_HTTP_HANDLER_H_