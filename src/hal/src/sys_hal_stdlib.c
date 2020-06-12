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

#include "sys_hal_config_def.h"
#include "uni_sys_hal.h"

void *uni_hal_malloc(size_t size) {
  return malloc(size);
}

void uni_hal_free(void *ptr) {
  free(ptr);
}

void *uni_hal_calloc(size_t n, size_t size) {
  return calloc(n, size);
}

long int uni_hal_strtol(const char *nptr, char **endptr, int base) {
  return strtol(nptr, endptr, base);
}

long long int uni_hal_strtoll(const char *nptr, char **endptr, int base) {
  return strtoll(nptr, endptr, base);
}

float uni_hal_strtof(const char *nptr, char **endptr) {
  return strtof(nptr, endptr);
}

double uni_hal_strtod(const char *nptr, char **endptr) {
  return strtod(nptr, endptr);
}
