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
 * Description : uni_memcheck.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2019.03.04
 *
 **************************************************************************/

#include "uni_memcheck.h"

#include "uni_iot.h"
#include "list_head.h"

#define MEMCHECK_HEAD_SIZE (sizeof(unsigned int))
#define MEMCHECK_TAIL_SIZE (sizeof(unsigned int))

/**   ----------------------------------------------------------------
  *   |                 MEMCHECK HEAP ALLOC LAYOUT                   |
  *   ----------------------------------------------------------------
  *   |  INDEX  | struct MemCheckItem | customer alloc size | ~INDEX |
  *   ----------------------------------------------------------------
  *   |  4 Byte |      16 Byte        |     align 4 Byte    | 4 Byte |
  *   ----------------------------------------------------------------
  **/

#if UNI_MEMCHECK_ENABLE

typedef struct {
  list_head    link;
  void         *customer_alloc_ptr;
  unsigned int customer_alloc_size;
} MemCheckItem;

typedef struct {
  list_head       mem_list;
  uint32_t        index;
  pthread_mutex_t mutex;
  unsigned int    current_cnt;
} MemCheckInf;

static MemCheckInf g_memcheckinf;

static void _print_one(MemCheckItem *item) {
  unsigned int *first_tag, *last_tag;
  first_tag = (unsigned int *)((char *)item - MEMCHECK_HEAD_SIZE);
  last_tag = (unsigned int *)(((char *)item) + (item->customer_alloc_size +
                                                sizeof(MemCheckItem)));
  printf("[%u %u %u] ", *first_tag, item->customer_alloc_size, *last_tag);
  if (~(*first_tag) != *last_tag) {
    printf("first_tag[%u] last_tag[%u] diff\n", *first_tag, *last_tag);
  }
}

static void _memcheck_print_tsk() {
  MemCheckItem *item;
  int print_cnt = 0;
  int total_size = 0;
  sleep(30); /* wait all modules inited */
  while (1) {
    print_cnt = 0;
    total_size = 0;
    pthread_mutex_lock(&g_memcheckinf.mutex);
    printf("----------------heap status----------------\n");
    printf("current_cnt=%d\n", g_memcheckinf.current_cnt);
    list_for_each_entry(item, &g_memcheckinf.mem_list, MemCheckItem, link) {
      _print_one(item);
      total_size += item->customer_alloc_size;
      if (print_cnt++ % 8 == 7) {
        printf("\n");
      }
    }
    printf("\n");
    printf("------[current used heap size=%d KB]-------\n", total_size / 1024);
    pthread_mutex_unlock(&g_memcheckinf.mutex);
    sleep(5);
  }
}

int MemCheckInit() {
  pthread_t pid;
  list_init(&g_memcheckinf.mem_list);
  pthread_mutex_init(&g_memcheckinf.mutex, NULL);
  pthread_create(&pid, NULL, _memcheck_print_tsk, NULL);
  pthread_detach(pid);
  return 0;
}

static void _destroy_mem_list() {
  MemCheckItem *p, *n;
  char *alloc_ptr;
  pthread_mutex_lock(&g_memcheckinf.mutex);
  list_for_each_entry_safe(p, n, &g_memcheckinf.mem_list, MemCheckItem, link) {
    alloc_ptr = (char *)p - MEMCHECK_HEAD_SIZE;
    list_del(&p->link);
    free(alloc_ptr);
  }
  pthread_mutex_unlock(&g_memcheckinf.mutex);
}

void MemCheckFinal() {
  _destroy_mem_list();
  pthread_mutex_destroy(&g_memcheckinf.mutex);
  return;
}

void* MemCheckMalloc(size_t size) {
  MemCheckItem *item = NULL;
  unsigned int *first_index_at_alloc_begin = NULL;
  unsigned int *last_index_at_alloc_tail = NULL;
  size /= 4;
  size += 1;
  size *= 4;
  char *p = (char *)malloc(MEMCHECK_HEAD_SIZE + sizeof(MemCheckItem) + size +
                           MEMCHECK_TAIL_SIZE);
  first_index_at_alloc_begin = (unsigned int *)p;
  last_index_at_alloc_tail = (unsigned int *)(p + (MEMCHECK_HEAD_SIZE +
                                              sizeof(MemCheckItem) + size));
  item = (MemCheckItem *)(p + MEMCHECK_HEAD_SIZE);
  item->customer_alloc_ptr = p + (MEMCHECK_HEAD_SIZE + sizeof(MemCheckItem));
  item->customer_alloc_size = size;
  pthread_mutex_lock(&g_memcheckinf.mutex);
  *first_index_at_alloc_begin = ++g_memcheckinf.index;
  *last_index_at_alloc_tail = ~g_memcheckinf.index;
  g_memcheckinf.current_cnt++;
  list_add(&item->link, &g_memcheckinf.mem_list);
  pthread_mutex_unlock(&g_memcheckinf.mutex);
  return item->customer_alloc_ptr;
}

void MemCheckFree(void *ptr) {
  char *p = (char *)ptr;
  unsigned int *first_index_at_alloc_begin = NULL;
  unsigned int *last_index_at_alloc_tail = NULL;
  MemCheckItem *item = (MemCheckItem *)(p - sizeof(MemCheckItem));
  first_index_at_alloc_begin = (unsigned int *)(p - (MEMCHECK_HEAD_SIZE +
                                                     sizeof(MemCheckItem)));
  last_index_at_alloc_tail = (unsigned int *)(p + item->customer_alloc_size);
  if (~(*first_index_at_alloc_begin) != *last_index_at_alloc_tail) {
    printf("[%u-->%u] diff, fatal heap error\n",
           *first_index_at_alloc_begin, *last_index_at_alloc_tail);
    return;
  }
  pthread_mutex_lock(&g_memcheckinf.mutex);
  list_del(&item->link);
  g_memcheckinf.current_cnt--;
  pthread_mutex_unlock(&g_memcheckinf.mutex);
  free(first_index_at_alloc_begin);
}

#else

void *MemCheckMalloc(size_t size) {
  return uni_malloc(size);
}

void MemCheckFree(void *ptr) {
  uni_free(ptr);
}

int MemCheckInit(void) {
  return 0;
}

void MemCheckFinal(void) {
  return;
}

#endif
