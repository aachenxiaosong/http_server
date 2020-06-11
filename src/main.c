#include "log.h"
#include "serial.h"
#include "http_server.h"
#include "http_request_handler.h"
#include "rili_call_lift.h"

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
  HttpRequestHandler *handler = new RiliCallLift();
  http_server_add_handler(handler);
  while(1) {
    sleep(100);    
  }
  serial_release();
  return 0;
}