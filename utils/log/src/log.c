#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>

#define DBG_BG_RED                  "\033[41m"
#define DBG_END                     "\033[0m"

static void _get_now_str(char *buf, int len) {
  struct timeval tv;
  time_t s;
  struct tm local;
  gettimeofday(&tv, NULL);
  s = tv.tv_sec;
  localtime_r(&s, &local);
  snprintf(buf, len, "%02d:%02d:%02d.%06"PRId64" ", local.tm_hour,
           local.tm_min, local.tm_sec, (int64_t)tv.tv_usec);
}

void mylog(int is_fatal, const char *file, int line, const char *format, ...) {
  char current[64] = {0};
  _get_now_str(current, 64);
  if (is_fatal) {
    printf("%s[E]%s ", DBG_BG_RED, DBG_END);
  } else {
    printf("[T] ");
  }
  printf("%s%s:%d ", current, file, line);
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
  printf("\n");
}
