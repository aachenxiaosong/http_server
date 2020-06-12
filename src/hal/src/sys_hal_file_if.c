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

#include "sys_hal_config_def.h"
#include "uni_sys_hal.h"

int uni_hal_fopen(const char *pathname, int flags, ...) {
  int fd;
  mode_t mode = 0;
  va_list args;

  if (O_CREAT & flags) {
    va_start(args, flags);
    mode = (mode_t)va_arg(args, int);
    va_end(args);

    fd = open(pathname, flags, mode);
  } else {
    fd = open(pathname, flags);
  }

  return fd;
}

ssize_t uni_hal_fwrite(int fd, const void *buf, size_t count) {
  return write(fd, buf, count);
}

ssize_t uni_hal_fread(int fd, void *buf, size_t count) {
  return read(fd, buf, count);
}

int uni_hal_fclose(int fd) {
  return close(fd);
}

int uni_hal_lseek(int fd, long offset, int whence) {
  return lseek(fd, offset, whence);
}

int uni_hal_unlink(const char *pathname) {
  return unlink(pathname);
}
