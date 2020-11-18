#include "uni_iot.h"
#include "uni_log.h"
#include "serial.h"
#include "uni_timer.h"
#include "HttpServer.hpp"
#include "Sulink.hpp"
#include "LiftCtrl.hpp"

#define MAIN_TAG "main"

static int _common_init() {
    srand(time(0));
    if (E_OK != TimerInitialize()) {
        LOGE(MAIN_TAG, "timer init failed");
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
#else
    //基础模块初始化
    if (0 != _common_init()) {
        LOGE(MAIN_TAG, "common init failed");
        return -1;
    }
    //串口
    if (0 != serial_init()) {
        LOGE(MAIN_TAG, "uart init failed");
        //return -1;
    }
    //http server
    if(0 != http_server_start()) {
        LOGE(MAIN_TAG, "http server init failed");
        return -1;
    }
    //sulink
    Sulink sulink;
    sulink.init();
    //lift control
    LiftCtrl lift_ctrl;
    lift_ctrl.start();  
    while(1) {
        sleep(100);
    }
    serial_release();
#endif
    return 0;
}

