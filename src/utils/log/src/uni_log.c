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
 * Description : uni_log.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_log.h"
#include "uni_iot.h"

#define BUF_DEFAULT_LEN (10240 * 5)

static const char* _level_tostring(LogLevel level) {
  switch (level) {
    case N_LOG_ERROR: return "[E]";
    case N_LOG_DEBUG: return "[D]";
    case N_LOG_TRACK: return "[T]";
    case N_LOG_WARN:  return "[W]";
    default:          return "[N/A]";
  }
}

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

static void _get_thread_id_str(char *buf, int len) {
  pthread_t thread_id = pthread_self();
  snprintf(buf, len, "%x", (unsigned int)thread_id);
}

static int _fill_log_level(LogLevel level, char *buf, int len) {
  int write_len = 0;
  switch (level) {
    case N_LOG_DEBUG:
      write_len = snprintf(buf, len, "\033[0m\033[47;33m%s\033[0m ",
                           _level_tostring(N_LOG_DEBUG));
      break;
    case N_LOG_TRACK:
      write_len = snprintf(buf, len, "\033[0m\033[42;33m%s\033[0m ",
                           _level_tostring(N_LOG_TRACK));
      break;
    case N_LOG_WARN:
      write_len = snprintf(buf, len, "\033[0m\033[41;33m%s\033[0m ",
                           _level_tostring(N_LOG_WARN));
      break;
    case N_LOG_ERROR:
      write_len = snprintf(buf, len, "\033[0m\033[41;33m%s\033[0m ",
                           _level_tostring(N_LOG_ERROR));
      break;
    default:
      break;
  }
  return uni_max(0, write_len);
}

static int _fill_tag(char *buf, int len, const char *tag) {
  return uni_max(0, snprintf(buf, len, "<%s>", tag));
}

static int _fill_time(char *buf, int len) {
  char now[64];
  _get_now_str(now, sizeof(now));
  return uni_max(0, snprintf(buf, len, "%s", now));
}

static int _fill_function_line(char *buf, int len, const char *function,
                               int line) {
  return (uni_max(0, snprintf(buf, len, "%s:%d->", function, line)));
}

static int _fill_thread_id(char *buf, int len) {
  char thread_id[32];
  _get_thread_id_str(thread_id, sizeof(thread_id));
  return uni_max(0, snprintf(buf, len, "%s", thread_id));
}

static void _fill_customer_info(char *buf, int len, char *fmt, va_list args,
                                int append_feed_line) {
  int length, remain_len;
  length = vsnprintf(buf, len, fmt, args);
  length = uni_max(length, 0);
  length = uni_min(length, len);
  remain_len = len - length;
  if (0 == remain_len) {
    if (append_feed_line) {
      buf[len - 2] = '\n';
    }
    buf[len - 1] = '\0';
    return;
  }
  if (1 == remain_len) {
    if (append_feed_line) {
      buf[len - 2] = '\n';
    }
    return;
  }
  if (append_feed_line) {
    strncat(buf, "\n", remain_len);
  }
  return;
}

#define _log_assemble(buf, level, tags, function, line, fmt, args) do { \
  int len = 0; \
  if (level != N_LOG_RAW) { \
    len += _fill_log_level(level, buf + len, BUF_DEFAULT_LEN - len); \
    len += _fill_time(buf + len, BUF_DEFAULT_LEN - len); \
    len += _fill_thread_id(buf + len, BUF_DEFAULT_LEN - len); \
    len += _fill_tag(buf + len, BUF_DEFAULT_LEN - len, tags); \
    len += _fill_function_line(buf + len, BUF_DEFAULT_LEN - len, \
                               function, line); \
  } \
  _fill_customer_info(buf + len, BUF_DEFAULT_LEN - len, fmt, args, \
                      level != N_LOG_RAW); \
} while (0)

static void _sync_write_process(LogLevel level, const char *tags,
                                const char *function, int line,
                                char *fmt, va_list args) {
  char *buffer = (char *)uni_malloc(BUF_DEFAULT_LEN);
  if (NULL == buffer) {
    printf("%s%d: alloc memory failed.\n", __FUNCTION__, __LINE__);
    return;
  }
  _log_assemble(buffer, level, tags, function, line, fmt, args);
  printf("%s", buffer);
  uni_free(buffer);
}

int LogWrite(LogLevel level, const char *tags, const char *function, int line,
             char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _sync_write_process(level, tags, function, line, fmt, args);
  va_end(args);
  return 0;
}

