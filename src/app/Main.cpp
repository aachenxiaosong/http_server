#include "uni_iot.h"
#include "uni_log.h"
#include "serial.h"
#include "uni_timer.h"
#include "ClientCommon.hpp"
#include "HttpServer.hpp"
#include "IHttpService.hpp"
#include "RiliHttpService.hpp"
#include "WlongHttpService.hpp"

#define MAIN_TAG "main"

static int _common_init() {
    if (E_OK != TimerInitialize()) {
        LOGE(MAIN_TAG, "timer init failed");
        return -1;
    }
    if (0 != ClientCommon::init()) {
        LOGE(MAIN_TAG, "client common init failed");
        return -1;
    }
    return 0;
}


int main() {
#if 0
    extern void AppTest();
    AppTest();
    while(1) {
        sleep(100); 
    }
    srand(time(0));
#else
    //串口
    if (0 != serial_init()) {
        LOGE(MAIN_TAG, "uart init failed");
        //return -1;
    }  
    if(0 != http_server_start()) {
        LOGE(MAIN_TAG, "http server init failed");
        return -1;
    }
    if (0 != _common_init()) {
        LOGE(MAIN_TAG, "common init failed");
        return -1;
    }
    serial_release();
#endif
    return 0;
}

