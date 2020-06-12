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

typedef void* (*FUNC_1)(void *);

struct thread_info {
  pthread_t th_id;
};

int uni_hal_thread_create(void **thread, struct thread_param *para,
                          void* (*start_routine)(void *), void *arg) {
  int val = 0;
  int th_priority;
  struct sched_param param;

  // pthread_t th_id;
  FUNC_1 func = (FUNC_1)start_routine;
  struct thread_info *info =
      (struct thread_info *)malloc(sizeof(struct thread_info));

  HAL_PRINT_INFO("%s----\n", __func__);

  pthread_attr_t attr;

  int ret = pthread_attr_init(&attr);
  if (ret != 0) {
    HAL_PRINT_ERR("%s pthread_attr_init \n", __func__);
    return -1;
  }

  if (para != NULL && para->stack_size > 16 * 1024) {
    ret = pthread_attr_setstacksize(&attr, para->stack_size);
    if (ret != 0) {
      HAL_PRINT_ERR("%s pthread_attr_setstacksize \n", __func__);
      return -1;
    }
  }
  if (para == NULL) {
    th_priority = 50;

  } else if (OS_PRIORITY_REALTIME == para->th_priority) {
    th_priority = 99;
  } else if (OS_PRIORITY_HIGH == para->th_priority) {
    th_priority = 70;
  } else if (OS_PRIORITY_NORMAL == para->th_priority) {
    th_priority = 50;
  } else if (OS_PRIORITY_LOW == para->th_priority) {
    th_priority = 30;
  } else if (OS_PRIORITY_IDLE == para->th_priority) {
    th_priority = 1;
  }
  param.sched_priority = th_priority;
  pthread_attr_setschedpolicy(&attr, SCHED_RR);
  pthread_attr_setschedparam(&attr, &param);

  *thread = info;
  val = pthread_create(&info->th_id, &attr, func, arg);
  if (val != 0) {
    HAL_PRINT_ERR("%s, Ctread thread failed.\n", __func__);
    return -1;
  }
  HAL_PRINT_ERR("pthread_create success\n");

  return 0;
}

int uni_hal_thread_detach(void *thread) {
  int ret = 0;
  struct thread_info *info = (struct thread_info *)thread;
  HAL_PRINT_ERR("%s---- %d\n", __func__, info->th_id);
  ret = pthread_detach(info->th_id);
  HAL_PRINT_ERR("%s---- %d   %d  end\n", __func__, info->th_id, ret);
  return ret;
}

int uni_hal_thread_destroy(void *thread) {
  struct thread_info *info = (struct thread_info *)thread;
  free(info);
  return 0;
}
