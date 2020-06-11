#include "log.h"
#include "serial.h"
#include "http_server.h"

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
  while(1) {
    sleep(100);    
  }
  serial_release();
  return 0;
}