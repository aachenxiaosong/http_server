#include "WlongHttpService.hpp"
#include "HttpServer.hpp"
#include "uni_log.h"

#define WLONG_SERVICE_TAG "wlong_http_service"

WlongHttpService :: WlongHttpService() : 
    IHttpService("wlong_http_service") {
    LOGT(WLONG_SERVICE_TAG, "wlong http service created");
}

int WlongHttpService :: registerHttpHandler() {
    IHttpRequestHandler *handler;
    handler = &mCallLiftHandler;
    http_server_add_handler(handler);
    handler = &mCallLiftOutHandler;
    http_server_add_handler(handler);
    handler = &mBookLiftHandler;
    http_server_add_handler(handler);
    handler = &mBookLift1Handler;
    http_server_add_handler(handler);
    handler = &mTakeLiftHandler;
    http_server_add_handler(handler);
    return 0;
}

 int WlongHttpService :: unRegisterHttpHandler() {
     http_server_clear_handler();
 }
