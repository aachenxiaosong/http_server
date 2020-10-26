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
 * Description : uni_time_tick.h
 * Author      : panyunxiang.unisound.com
 * Date        : 2020.08.19
 *
 **************************************************************************/
#ifndef UTILS_TIME_TICK_INC_UNI_TIMER_H_
#define UTILS_TIME_TICK_INC_UNI_TIMER_H_

#include "uni_iot.h"


typedef struct {
  uni_s32 year;
  uni_s32 month;
  uni_s32 day;
  uni_s32 hour;
  uni_s32 minute;
  uni_s32 second;
  uni_s32 wday;
} TimerDate;

uni_s32 uni_get_datetime_unix(TimerDate *date);
uni_s32 uni_sec_to_datetime(TimerDate *date, uni_u32 sec);
uni_s64 uni_datetime_to_msec(TimerDate *date);
uni_u32 uni_datetime_to_sec(TimerDate *date);
uni_bool uni_is_leap_year(TimerDate *date);
uni_u32 uni_get_today_sec_unix();
uni_s64 uni_get_now_msec(void);
uni_u32 uni_get_now_sec_unix(void);
uni_u32 uni_get_uptime_sec_unix();
long uni_get_clock_time_ms(void);










#endif
