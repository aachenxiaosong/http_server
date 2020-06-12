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

struct mutex_info {
  pthread_mutex_t mutex;
};

int uni_hal_mutex_init(void **mutex) {
  // int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *
  // attr)

  struct mutex_info *info =
      (struct mutex_info *)malloc(sizeof(struct mutex_info));

  pthread_mutex_init(&info->mutex, NULL);
  *mutex = info;

  return 0;
}

int uni_hal_mutex_lock(void *mutex) {
  struct mutex_info *info = (struct mutex_info *)mutex;
  pthread_mutex_lock(&info->mutex);

  return 0;
}

int uni_hal_mutex_unlock(void *mutex) {
  struct mutex_info *info = (struct mutex_info *)mutex;
  pthread_mutex_unlock(&info->mutex);

  return 0;
}
int uni_hal_mutex_destroy(void *mutex) {
  struct mutex_info *info = (struct mutex_info *)mutex;
  free(info);

  return 0;
}
