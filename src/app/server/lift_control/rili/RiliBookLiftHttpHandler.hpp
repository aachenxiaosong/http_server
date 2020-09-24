#ifndef SERVER_LIFT_CONTROL_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_
#define SERVER_LIFT_CONTROL_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_

#include "IBookLiftHttpHandler.hpp"
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class RiliBookLiftHttpHandler :  public IBookLiftHttpHandler {
    private:
    int handleRequest(CJsonObject& jrequest, CJsonObject& jresponse);
    public:
    RiliBookLiftHttpHandler();
};

#endif  //  SERVER_LIFT_CONTROL_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_