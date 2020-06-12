#include "uni_log.h"
#include "serial.h"
#include "HttpServer.hpp"
#include "IHttpService.hpp"
#include "RiliHttpService.hpp"
#include "WlongHttpService.hpp"

#define MAIN_TAG "main"

int main() { 
    //串口
    if (0 != serial_init()) {
        LOGE(MAIN_TAG, "uart init failed");
        //return -1;
    }
    if(0 != http_server_start()) {
        LOGE(MAIN_TAG, "http server init failed");
        //return -1;
    }
    
    IHttpService *http_service = new WlongHttpService();
    http_service->registerHttpHandler();
    while(1) {
        sleep(100); 
    }
    delete http_service;
    serial_release();
    return 0;
}

