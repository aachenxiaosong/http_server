#include "WlongHttpService.hpp"
#include "HttpServer.hpp"
#include "log.h"

WlongHttpService :: WlongHttpService() : 
    IHttpService("wlong_http_service") {
    LOGT("wlong http service created");
}

int WlongHttpService :: registerHttpHandler() {
    IHttpRequestHandler *handler;
    handler = &mCallLiftHandler;
    http_server_add_handler(handler);
    handler = &mBookLiftHandler;
    http_server_add_handler(handler);
    handler = &mTakeLiftHandler;
    http_server_add_handler(handler);
    return 0;
}