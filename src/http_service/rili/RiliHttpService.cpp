#include "RiliHttpService.hpp"
#include "HttpServer.hpp"
#include "log.h"

RiliHttpService :: RiliHttpService() : 
    IHttpService("rili_http_service") {
    LOGT("rili http service created");
}

int RiliHttpService :: registerHttpHandler() {
    IHttpRequestHandler *handler;
    handler = &mCallLiftHandler;
    http_server_add_handler(handler);
    handler = &mBookLiftHandler;
    http_server_add_handler(handler);
    handler = &mTakeLiftHandler;
    http_server_add_handler(handler);
    return 0;
}