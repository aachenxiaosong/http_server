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


#define uni_max(a, b)                    ((a) < (b) ? (b) : (a))
#define uni_min(a, b)                    ((b) < (a) ? (b) : (a))
#define uni_abs(a, b)                    ((a) > (b) ? ((a)-(b)) : ((b)-(a)))

static inline void *uni_malloc(size_t size) {
  return malloc(size);
}
static inline void uni_free(void *ptr) {
  free(ptr);
}

#ifdef __cplusplus
}
#endif
#endif
