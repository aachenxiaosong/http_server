#ifndef UART_INC_SERIAL_H_
#define UART_INC_SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

int serial_recv(unsigned char* buf, int *n);
int serial_send(unsigned char* buf, int n);

int serial_init();
void serial_release();

#ifdef __cplusplus
}
#endif
#endif  //  UART_INC_SERIAL_H_

