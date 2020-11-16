#ifndef LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_
#define LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_

#include "IBookLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;
//围墙机刷脸,合并到booklift接口
class WlongBookLift1HttpHandler :  public IBookLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    WlongBookLift1HttpHandler();
};

#endif  //  LEFT_LIFT_CONTROL_WLONG_HTTP_WLONG_BOOK_LIFT1_HTTP_HANDLER_H_