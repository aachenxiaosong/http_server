#ifndef LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT_HTTP_HANDLER_H_
#define LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT_HTTP_HANDLER_H_

#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class WlongBookLiftHttpHandler :  public IBookLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLiftHttpHandler();
};

#endif  //  LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT_HTTP_HANDLER_H_