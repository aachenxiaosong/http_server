#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "log.h"

#define BUF_MAX (1024 * 16)

static unsigned char _calculate_sum(unsigned char *frame, int len) {
  int i;
  unsigned char sum = 0;
  for (i = 1; i < len - 2; i++) {
    sum += frame[i];
  }
  return sum;
}

static void _printf_frame(unsigned char *frame, int len) {
  int i;
  printf("frame: ");
  for (i = 0; i < len; i++) {
    printf("0x%x ", frame[i]);
  }
  printf("\n");
}

void assemble_ack(unsigned char *ack, unsigned char request_id,
                  unsigned char event_type, unsigned char lift_num) {
  static int i = 0;
  memset(ack, 0, 10);
  ack[0] = 0x02;
  ack[1] = event_type | (request_id << 4);
  if (event_type == 7) {
    ack[2] = lift_num;
    ack[3] = 4;
    ack[7] = 5;
    ack[8] = ack[1] + ack[2] + ack[3] + ack[7];
  } else {
    ack[2] = (i % 2);
    ack[8] = ack[1] + ack[2];
  }
  ack[9] = 0x03;
  i++;
}

int main() {
  unsigned char frame[128];
  int n;
  unsigned char check_sum;
  unsigned char event_type;
  unsigned char request_id;
  unsigned char lift_num;
  unsigned char ack[10];
  //串口
  if (0 != serial_init()) {
    LOGE("uart init failed");
    //return -1;
  }
  while (1) {
    usleep(1000 * 50);
    if (0 == serial_recv(frame, &n)) {
      check_sum = _calculate_sum(frame, n);
      _printf_frame(frame, n);
      if (frame[0] != 0x02 || frame[n - 1] != 0x03) {
        LOGE("wrong header 0x%x or tail 0x%x", frame[0], frame[n - 1]);
        continue;
      }
      if (frame[n - 2] != check_sum) {
        LOGE("wrong check sum");
        continue;
      }
      event_type = frame[5] & 0x0f;
      request_id = frame[5] >> 4;
      LOGT("frame[5]:0x%x event:%d request_id:%d", frame[5], event_type, request_id);
      lift_num = frame[6];
      assemble_ack(ack, request_id, event_type, lift_num);
      serial_send(ack, 10);
    }
  } 
  serial_release();

  return 0;
}
