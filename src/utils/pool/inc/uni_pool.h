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
 * Description : uni_pool.h
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef UTILS_POOL_INC_UNI_POOL_H_
#define UTILS_POOL_INC_UNI_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

typedef void *(*FUNC)(void *arg);

typedef struct thpool_job_t {
  FUNC                function;
  void                *arg;
  struct thpool_job_t *prev;
  struct thpool_job_t *next;
} thpool_job_t;

typedef struct {
  thpool_job_t *head;
  thpool_job_t *tail;
  int          jobN;
  uni_sem_t    queueSem;
} thpool_jobqueue;

typedef struct {
  uni_pthread_t   *threads;
  int             threadsN;
  thpool_jobqueue *jobqueue;
  uni_sem_t       threadStop;
} thpool_t;

typedef struct {
  uni_mutex_t mutex_p;
  thpool_t    *tp_p;
} thread_data;

thpool_t*     thpool_init(int thread_num);
int           thpool_add_work(thpool_t *tp_p, void *(*function_p)(void *), void *arg_p);
void          thpool_destroy(thpool_t *tp_p);
int           thpool_jobqueue_init(thpool_t *tp_p);
void          thpool_jobqueue_add(thpool_t *tp_p, thpool_job_t *newjob_p);
int           thpool_jobqueue_removelast(thpool_t *tp_p);
thpool_job_t* thpool_jobqueue_peek(thpool_t *tp_p);
void          thpool_jobqueue_empty(thpool_t *tp_p);

#ifdef __cplusplus
}
#endif
#endif
