#include "RiliHttpService.hpp"
#include "HttpServer.hpp"
#include "UniLog.hpp"

#define RILI_SERVICE_TAG "rili_http_service"

RiliHttpService :: RiliHttpService() : 
    IHttpService("rili_http_service") {
    LOGT(RILI_SERVICE_TAG, "rili http service created");
}

int RiliHttpService :: registerHttpHandler() {
    IHttpRequestHandler *handler;
    handler = &mCallLiftHandler;
    http_server_add_handler(handler);
    handler = &mBookLiftHandler;
    http_server_add_handler(handler);
    //handler = &mTakeLiftHandler;
    //http_server_add_handler(handler);
    return 0;
}

 int RiliHttpService :: unRegisterHttpHandler() {
     http_server_clear_handler();
 }