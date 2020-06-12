#include "log.h"
#include "serial.h"
#include "HttpServer.hpp"
#include "IHttpService.hpp"
#include "RiliHttpService.hpp"


int main() { 
    //串口
    if (0 != serial_init()) {
        LOGE("uart init failed");
        //return -1;
    }
    if(0 != http_server_start()) {
        LOGE("http server init failed");
        //return -1;
    }
    
    IHttpService *http_service = new RiliHttpService();
    http_service->registerHttpHandler();
    while(1) {
        sleep(100); 
    }
    delete http_service;
    serial_release();
    return 0;
}

