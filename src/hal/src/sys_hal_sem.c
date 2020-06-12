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

struct sem_info {
  sem_t sem;
};

int uni_hal_sem_init(void **sem, unsigned int value) {
  struct sem_info *info = (struct sem_info *)malloc(sizeof(struct sem_info));

  sem_init(&info->sem, 0, 0);
  // pthread_mutex_init(&info->mutex, NULL);
  *sem = info;
  return 0;
}
int uni_hal_sem_wait(void *sem) {
  struct sem_info *info = (struct sem_info *)sem;
  sem_wait(&info->sem);

  return 0;
}
int uni_hal_sem_post(void *sem) {
  struct sem_info *info = (struct sem_info *)sem;
  sem_post(&info->sem);

  return 0;
}
int uni_hal_sem_destroy(void *sem) {
  struct sem_info *info = (struct sem_info *)sem;
  sem_destroy(&info->sem);
  free(info);
  return 0;
}
