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
 * Description : uni_uuid.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.06.19
 *
 **************************************************************************/
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "uni_uuid.h"

#define CHARACTOR_NUM 36
static const char *c_charactors = "0123456789abcdefghijklmnopqrstuvwxyz";

int GetUuid(char *uuid) {
  if (NULL == uuid) {
    return -1;
  }
  int i;
  for (i = 0; i < UUID_LEN; i++) {
    uuid[i] = c_charactors[rand() % CHARACTOR_NUM];
  }
  uuid[i] = '\0';
  return 0;
}
