#ifndef SERVICE_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_
#define SERVICE_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"

using namespace std;

class RiliBookLiftHttpHandler :  public IHttpRequestHandler {
    public:
    RiliBookLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  SERVICE_RILI_RILI_BOOK_LIFT_HTTP_HANDLER_H_