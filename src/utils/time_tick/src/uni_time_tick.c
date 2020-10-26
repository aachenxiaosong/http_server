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
 * Description : uni_time_tick.c
 * Author      : panyunxiang.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_time_tick.h"

/* get  clock */
uni_s32 uni_get_datetime_unix(TimerDate *date) {
  time_t timeval;
  struct tm tm;
  time(&timeval);
  localtime_r(&timeval, &tm);

  date->year = tm.tm_year + 1900;
  date->month = tm.tm_mon + 1;
  date->day = tm.tm_mday;
  date->hour = tm.tm_hour;
  date->minute = tm.tm_min;
  date->second = tm.tm_sec;

  return 0;
}

/* timestamp to clock */
uni_s32 uni_sec_to_datetime(TimerDate *date, uni_u32 sec) {
  time_t time = sec;
  struct tm *ptm = NULL;

  ptm = localtime(&time);

  date->year = ptm->tm_year + 1900;
  date->month = ptm->tm_mon + 1;
  date->day = ptm->tm_mday;
  date->hour = ptm->tm_hour;
  date->minute = ptm->tm_min;
  date->second = ptm->tm_sec;
  
  return 0;
}

/*   clock to timestamp (ms)  */
uni_s64 uni_datetime_to_msec(TimerDate *date) {
  struct tm t;
  uni_s64 sec;
  uni_memset(&t, 0, sizeof(struct tm));
  t.tm_year = date->year - 1900;
  t.tm_mon = date->month - 1;
  t.tm_mday = date->day;
  t.tm_hour = date->hour;
  t.tm_min = date->minute;
  t.tm_sec = date->second;
  sec = mktime(&t);
  return sec * 1000;
}


/*   clock to timestamp (s)  */
uni_u32 uni_datetime_to_sec(TimerDate *date) {
  struct tm t;
  uni_u32 sec;
  uni_memset(&t, 0, sizeof(struct tm));
  t.tm_year = date->year - 1900;
  t.tm_mon = date->month - 1;
  t.tm_mday = date->day;
  t.tm_hour = date->hour;
  t.tm_min = date->minute;
  t.tm_sec = date->second;
  sec = mktime(&t);
  return sec;
}

uni_bool uni_is_leap_year(TimerDate *date) {
  return ((date->year % 100 != 0 && 0 == date->year % 4) ||
          (0 == date->year % 400));
}


/* timestamp of today 00:00:00 */
uni_u32 uni_get_today_sec_unix() {
  time_t timeval;
  struct tm local;
  time(&timeval);
  localtime_r(&timeval, &local);
  local.tm_hour = local.tm_min = local.tm_sec = 0;
  return mktime(&local);
}

/* timestamp from 1900  (ms) */
uni_s64 uni_get_now_msec(void) {
  struct timeval t1;
  gettimeofday(&t1, NULL);
  return ((uni_s64)t1.tv_sec * 1000 + t1.tv_usec/1000);
}

/* timestamp from 1970  (s) */
uni_u32 uni_get_now_sec_unix(void) {
#ifdef PLATFORM_UNIONE
  return (uni_get_now_msec() - 28803877) / 1000;
#else
  time_t timeval;
  time(&timeval);
  return (uni_u32)timeval;
#endif
}

/* reboot uptime  (s) */
uni_u32 uni_get_uptime_sec_unix() {
  struct sysinfo info;
  sysinfo(&info);
  return info.uptime;
}

/* reboot uptime  (ms) */
long uni_get_clock_time_ms(void) {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
    return (ts.tv_sec * 1000L + (ts.tv_nsec / 1000000));
  }
  return 0;
}



