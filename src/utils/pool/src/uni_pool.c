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
 * Description : uni_pool.c
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_pool.h"

static int thpool_keepalive = 1;
static thpool_t *g_thpool;
static void* thpool_thread_do(void *args);
static uni_mutex_t mutex;

thpool_t* thpool_init(int threadN) {
  struct thread_param tpara;
  uni_memset(&tpara, 0, sizeof(struct thread_param));
  thpool_keepalive = 1;
  uni_pthread_mutex_init(&mutex);
  if (!threadN || threadN < 1) {
    threadN = 1;
  }
  g_thpool = (thpool_t *)uni_malloc(sizeof(thpool_t));
  if (g_thpool == NULL) {
    printf("uni_malloc thpool_t error");
    return NULL;
  }
  g_thpool->threadsN = threadN;
  g_thpool->threads = (uni_pthread_t *)uni_malloc(threadN *
                                                  sizeof(uni_pthread_t));
  if (NULL == g_thpool->threads) {
    printf("uni_malloc thpool->threads error");
    return NULL;
  }
  uni_sem_init(&g_thpool->threadStop, 0);
  if (thpool_jobqueue_init(g_thpool)) {
    return NULL;
  }
  int t;
  tpara.stack_size = STACK_DEFAULT_SIZE;
  tpara.th_priority = OS_PRIORITY_NORMAL;
  uni_strncpy(tpara.task_name, THPOOL_TASK, sizeof(tpara.task_name) - 1);
  for (t = 0; t < threadN ; t++) {
    uni_pthread_create(&g_thpool->threads[t], &tpara, thpool_thread_do, NULL);
  }
  return g_thpool;
}

void thpool_destroy(thpool_t *tp_p) {
  int i;
  thpool_keepalive = 0;
  uni_pthread_mutex_destroy(mutex);
  for (i = 0; i < (tp_p->threadsN); i++) {
    if (uni_sem_post(tp_p->jobqueue->queueSem)) {
      fprintf(stderr, "thpool_destroy(): Could not bypass sem_wait()\n");
    }
  }
  if (uni_sem_post(tp_p->jobqueue->queueSem) != 0) {
    fprintf(stderr, "thpool_destroy(): Could not destroy semaphore\n");
  }
  for (i = 0; i < (tp_p->threadsN); i++) {
    uni_sem_wait(tp_p->threadStop);
    uni_pthread_destroy(g_thpool->threads[i]);
  }
  thpool_jobqueue_empty(tp_p);
  uni_free(tp_p->threads);
  uni_sem_destroy(tp_p->jobqueue->queueSem);
  uni_sem_destroy(tp_p->threadStop);
  uni_free(tp_p->jobqueue);
  uni_free(tp_p);
}

int thpool_jobqueue_init(thpool_t *tp_p) {
  tp_p->jobqueue = (thpool_jobqueue *)uni_malloc(sizeof(thpool_jobqueue));
  if (tp_p->jobqueue == NULL) return -1;
  tp_p->jobqueue->tail = NULL;
  tp_p->jobqueue->head = NULL;
  tp_p->jobqueue->jobN = 0;
  uni_sem_init(&(tp_p->jobqueue->queueSem), 0);
  return 0;
}

static void* thpool_thread_do(void *args) {
  thpool_t *tp_p = g_thpool;
  while (thpool_keepalive == 1) {
    if (uni_sem_wait(tp_p->jobqueue->queueSem)) {
      perror("thpool_thread_do(): Waiting for semaphore");
      exit(1);
    }
    if (thpool_keepalive) {
      FUNC function;
      void *arg_buff;
      thpool_job_t *job_p;
      uni_pthread_mutex_lock(mutex);
      job_p = thpool_jobqueue_peek(tp_p);
      function = job_p->function;
      arg_buff = job_p->arg;
      if (thpool_jobqueue_removelast(tp_p)) {
        uni_pthread_mutex_unlock(mutex);
        return NULL;
      }
      uni_pthread_mutex_unlock(mutex);
      function(arg_buff);
      uni_free(job_p);
    } else {
      break;
    }
  }
  uni_sem_post(tp_p->threadStop);
  return NULL;
}

thpool_job_t* thpool_jobqueue_peek(thpool_t *tp_p){
  return tp_p->jobqueue->tail;
}

int thpool_jobqueue_removelast(thpool_t *tp_p) {
  if (NULL == tp_p) {
    return -1;
  }
  thpool_job_t *theLastJob;
  theLastJob = tp_p->jobqueue->tail;
  switch (tp_p->jobqueue->jobN) {
    case 0:
      return -1;
    case 1:
      tp_p->jobqueue->head = NULL;
      tp_p->jobqueue->tail = NULL;
      break;
    default:
      theLastJob->prev->next = NULL;
      tp_p->jobqueue->tail = theLastJob->prev;
  }
  (tp_p->jobqueue->jobN)--;
  return 0;
}

void thpool_jobqueue_add(thpool_t *tp_p, thpool_job_t *newjob_p) {
  newjob_p->next = NULL;
  newjob_p->prev = NULL;
  thpool_job_t *oldFirstJob;
  oldFirstJob = tp_p->jobqueue->head;
  switch (tp_p->jobqueue->jobN) {
    case 0:
      tp_p->jobqueue->head = newjob_p;
      tp_p->jobqueue->tail = newjob_p;
      break;
    default:
      oldFirstJob->prev = newjob_p;
      newjob_p->next = oldFirstJob;
      tp_p->jobqueue->head = newjob_p;
  }
  (tp_p->jobqueue->jobN)++;
  uni_sem_post(tp_p->jobqueue->queueSem);
  return;
}

int thpool_add_work(thpool_t *tp_p, void *(*function_p)(void *), void *arg_p) {
  thpool_job_t *newjob;
  newjob = (thpool_job_t *)uni_malloc(sizeof(thpool_job_t));
  if (NULL == newjob) {
    fprintf(stderr, "thpool_add_work(): Could not allocate memory for new job\n");
    exit(1);
  }
  newjob ->function = function_p;
  newjob ->arg = arg_p;
  uni_pthread_mutex_lock(mutex);
  thpool_jobqueue_add(tp_p, newjob);
  uni_pthread_mutex_unlock(mutex);
  return 0;
}

void thpool_jobqueue_empty(thpool_t *tp_p) {
  thpool_job_t *curjob;
  curjob = tp_p->jobqueue->tail;
  while (tp_p->jobqueue->jobN) {
    tp_p->jobqueue->tail = curjob->prev;
    uni_free (curjob);
    curjob = tp_p->jobqueue->tail;
    (tp_p->jobqueue->jobN)--;
  }
  tp_p->jobqueue->head = NULL;
  tp_p->jobqueue->tail = NULL;
}
