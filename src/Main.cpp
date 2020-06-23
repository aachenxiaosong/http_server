#include "uni_log.h"
#include "serial.h"
#include "uni_timer.h"
#include "HttpServer.hpp"
#include "uni_resource_prefetch.h"
#include "IHttpService.hpp"
#include "RiliHttpService.hpp"
#include "WlongHttpService.hpp"
#include "McBroker.hpp"
#include "WlongMcSubscriber.hpp"

#define MAIN_TAG "main"

static int _http_client_init() {
    if (E_OK != TimerInitialize()) {
        LOGE(MAIN_TAG, "timer init failed");
        return -1;
    }
    if (E_OK != ResPrefetchInit()) {
        LOGE(MAIN_TAG, "resource prefetch init failed");
        return -1;
    }
    return 0;
}

static int _msg_center_init() {
    if (E_OK != McBrokerInit()) {
        LOGE(MAIN_TAG, "mc service init failed");
        return -1;
    }
    if (E_OK != McBrokerConnect()) {
        LOGE(MAIN_TAG, "mc service connect failed");
        return -1;        
    }
    return 0;
}

int main() { 
    srand(time(0));
    //串口
    if (0 != serial_init()) {
        LOGE(MAIN_TAG, "uart init failed");
        //return -1;
    }
    if (0 != _msg_center_init()) {
        LOGE(MAIN_TAG, "http server init failed");
        return -1;
    }
    WlongMcSubscriber *wlong_subscriber = new WlongMcSubscriber();
    wlong_subscriber->registerMsgHandler();
    
    if(0 != http_server_start()) {
        LOGE(MAIN_TAG, "http server init failed");
        return -1;
    }
    if (0 != _http_client_init()) {
        LOGE(MAIN_TAG, "http client init failed");
        return -1;
    }
    
    while(1) {
        sleep(100); 
    }
    delete wlong_subscriber;
    serial_release();
    return 0;
}

