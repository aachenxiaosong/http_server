/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : uni_iot.h
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef HAL_INC_UNI_IOT_H_
#define HAL_INC_UNI_IOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/select.h>
#include <math.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/sysinfo.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <sys/poll.h>
#include <sys/reboot.h>
#include <sys/un.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
//#include <net/if.h>
#include <semaphore.h>
#include <float.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <time.h>
#include <sched.h>
#include <linux/types.h>
#include <linux/errqueue.h>
#include <linux/sockios.h>
#include <linux/wireless.h>
#include <signal.h>
#include <dirent.h>

#include "uni_types.h"
#include "uni_sys_hal.h"

#define UNI_CROW_VERSION      "v3.0.0--2020020411"

typedef enum {
  UNI_OPEN_ERR = 0x600,
  UNI_CLOSE_ERR,
  UNI_START_ERR,
  UNI_STOP_ERR,
  UNI_DEST_ERR,
  UNI_SUSP_ERR,
  UNI_THREAD_ERR,
  UNI_INVALID_HANDLE_ERR,
  UNI_INVALID_FILE_ERR,
  UNI_INVALID_SIZE_ERR,
  UNI_INVALID_FORMAT_ERR,
  UNI_INVALID_PARAM_ERR,
  UNI_NO_CMD_ERR,
  UNI_NO_MEM_ERR,
  UNI_NO_SUPPORT_ERR,
  UNI_MODULE_INIT_ERR,
  UNI_MODULE_UNINIT_ERR,
  UNI_MODULE_INIT_ALREADY_ERR,
  UNI_MODULE_BUSY_ERR,
  UNI_MODULE_NONE_ERR,
  UNI_INVALID_USERNAME_ERR,
  UNI_INVALID_PASSWORD_ERR,
  UNI_NO_PERMISION_ERR,
  UNI_GET_STATUS_ERR,
  UNI_SET_STATUS_ERR,
  UNI_ERR_NUM_END
} uni_com_err_t;

#define MEMBER_TO_STRUCT(member_ptr, \
                         type, member)   (type *)((char *)(member_ptr) - \
                                            (unsigned long)&((type *)0)->member)
#define UNI_ISSPACE(x)                   (((x) == 0x20) || ((x) > 0x8 && (x) < 0xe))
#define uni_assert                       assert
#define uni_max(a, b)                    ((a) < (b) ? (b) : (a))
#define uni_min(a, b)                    ((b) < (a) ? (b) : (a))
#define uni_abs(a, b)                    ((a) > (b) ? ((a)-(b)) : ((b)-(a)))
#define uni_printf                       printf
#define uni_sprintf                      sprintf
#define uni_perror                       perror
#define uni_sscanf                       sscanf
#define uni_snprintf                     snprintf

static inline void *uni_malloc(size_t size) {
  return malloc(size);
}
#define uni_calloc(n, size)              calloc(n, size)
static inline void uni_free(void *ptr) {
  free(ptr);
}

#define uni_strtol(nptr, endptr, base)   strtol(nptr, endptr, base)
#define uni_strtof(nptr, endptr)         strtof(nptr, endptr)
#define uni_strtoll(nptr, endptr, base)  strtoll(nptr, endptr, base)
#define uni_strtod(nptr, endptr)         strtod(nptr, endptr)

#define UNI_SEEK_SET                     (0)
#define UNI_SEEK_CUR                     (1)
#define UNI_SEEK_END                     (2)

#define uni_usleep(times)                usleep(times)
#define uni_msleep(times)                usleep(1000 * (times))
#define uni_sleep(times)                 sleep(times)

#define uni_memset(ptr, data, size)      memset(ptr, data, size)
#define uni_memcpy(dest, src, size)      memcpy(dest, src, size)
#define uni_strcat(dest, src)            strcat(dest, src)
#define uni_strncat(dest, src, size)     strncat(dest, src, size)
#define uni_strstr(haystack, needle)     strstr(haystack, needle)
#define uni_strrchr(s, c)                strrchr(s, c)
#define uni_strchr(s, c)                 strchr(s, c)
#define uni_strlen                       strlen
#define uni_strcpy                       strcpy
#define uni_strncpy                      strncpy
#define uni_strncmp                      strncmp
#define uni_strcmp                       strcmp

#define uni_isspace(ch)                  isspace(ch)

typedef void* uni_sem_t;

#define uni_sem_init(sem_ptrptr, value)  uni_hal_sem_init(sem_ptrptr, value)
#define uni_sem_post(sem_ptr)            uni_hal_sem_post(sem_ptr)
#define uni_sem_wait(sem_ptr)            uni_hal_sem_wait(sem_ptr)
#define uni_sem_destroy(sem_ptr)         uni_hal_sem_destroy(sem_ptr)

typedef void* uni_pthread_t;
#define uni_pthread_create(pthread_ptr,\
                           param, \
                           routine,\
                           args)         uni_hal_thread_create(pthread_ptr, \
                                                               param, \
                                                               routine, args)
#define uni_pthread_detach(th)           uni_hal_thread_detach(th)
#define uni_pthread_destroy(th)          uni_hal_thread_destroy(th)

typedef void* uni_mutex_t;
#define uni_pthread_mutex_init(uni_mutex_ptr)  uni_hal_mutex_init(uni_mutex_ptr)
#define uni_pthread_mutex_destroy(uni_mutex)   uni_hal_mutex_destroy(uni_mutex)
#define uni_pthread_mutex_lock(uni_mutex)      uni_hal_mutex_lock(uni_mutex)
#define uni_pthread_mutex_unlock(uni_mutex)    uni_hal_mutex_unlock(uni_mutex)

#ifdef __cplusplus
}
#endif
#endif
