#ifndef HTTP_SERVICE_WLONG_WLONG_BOOK_LIFT_HTTP_HANDLER_H_
#define HTTP_SERVICE_WLONG_WLONG_BOOK_LIFT_HTTP_HANDLER_H_

#include "IHttpRequestHandler.hpp"

using namespace std;

class WlongBookLiftHttpHandler :  public IHttpRequestHandler {
    public:
    WlongBookLiftHttpHandler();
    int handle(string& path, string& request, string&response) ;
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_BOOK_LIFT_HTTP_HANDLER_H_