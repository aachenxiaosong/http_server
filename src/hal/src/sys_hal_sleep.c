#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <stdarg.h>
#include "uni_sys_hal.h"

int uni_hal_usleep(unsigned int us) {
  return usleep(us);
}

int uni_hal_msleep(unsigned int ms) {
  // msleep(ms);
  return usleep(ms * 1000);
}

int uni_hal_sleep(unsigned int s) {
  return sleep(s);
}
