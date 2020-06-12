
#include "inc/uni_databuf.h"
#include <string.h>
#include <stdio.h>

int main() {
  char buf[256] = {0};
  char read_buf[256] = {0};

  DataBufHandle handle =  DataBufferCreate(1024);

  printf("------------------------case 1: sync after write------------------------------\n");
  snprintf(buf, sizeof(buf), "hello world");
  DataBufferWrite(handle, buf, strlen(buf) + 1, UNI_DATA_BUF_ATTR_WRITE);
  printf("%s%d: free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));
  DataBufferWrite(handle, buf, strlen(buf) + 1, UNI_DATA_BUF_ATTR_SYNC);
  printf("%s%d: free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));

  printf("------------------------case 2: write and sync------------------------------\n");
  DataBufferWrite(handle, buf, strlen(buf) + 1, UNI_DATA_BUF_ATTR_WRITE | UNI_DATA_BUF_ATTR_SYNC);
  printf("%s%d: free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));

  printf("------------------------case 3: sync after read------------------------------\n");
  DataBufferRead(read_buf, strlen(buf) + 1, handle, UNI_DATA_BUF_ATTR_READ);
  printf("%s%d: read_buf: %s\n", __FUNCTION__, __LINE__, read_buf);
  printf("%s%d: free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));
  DataBufferRead(read_buf, strlen(buf) + 1, handle, UNI_DATA_BUF_ATTR_SYNC);
  printf("%s%d: free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));

  printf("------------------------case 4: read and sync------------------------------\n");
  DataBufferReadSync(read_buf, strlen(buf) + 1, handle);
  printf("%s%d: read_buf: %s\n", __FUNCTION__, __LINE__, read_buf);
  printf("%s%d: free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));

  DataBufferDestory(handle);
  return 0;
}
