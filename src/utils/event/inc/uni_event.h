/**************************************************************************
 * Description : uni_event.h
 * Author      : yzs@unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef SDK_INC_UNI_EVENT_H_
#define SDK_INC_UNI_EVENT_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_SIZE                     sizeof(Event)
#define EVENT_SEQUENCE_ID_DEFAULT      (uint32_t)(-1)

struct EventContent;
typedef void (*EventContentFreeHandler)(struct EventContent *event_content);

typedef struct EventContent {
  void    *info;
  void    *extend_info;
  int     value_int;
  double  value_double;
  int     err_code;
} EventContent;

typedef struct {
  unsigned int            seq_id;
  int                     type;
  EventContent            content;
  EventContentFreeHandler free_handler;
} Event;

typedef int (*EventHandler)(Event *event);

/**
 * DefaultEventContentFreeHandler can only be used when info and extend_info
 * allocated by malloc.
 */
static inline void DefaultEventContentFreeHandler(EventContent *event_content) {
  if (NULL == event_content) {
    return;
  }
  if (NULL != event_content->info) {
    free(event_content->info);
    event_content->info = NULL;
  }
  if (NULL != event_content->extend_info) {
    free(event_content->extend_info);
    event_content->extend_info = NULL;
  }
}

Event* EventCreate(unsigned int seq_id, int type, EventContent *content,
                   EventContentFreeHandler free_handler);
void   EventFree(Event *event);

#ifdef __cplusplus
}
#endif
#endif  // SDK_INC_UNI_EVENT_H_
