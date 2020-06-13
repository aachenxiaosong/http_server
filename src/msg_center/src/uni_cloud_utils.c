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
 * Description : uni_cloud_utils.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2018.07.04
 *
 **************************************************************************/

#include "uni_cloud_utils.h"
#include "uni_sha.h"
#include "uni_log.h"

#define CLOUD_UTILS_TAG "cloud_utils"

static void _str_sort(const char *strs[], uni_s32 start, uni_s32 end) {
  uni_s32 i = start, j = end;
  const char *base = strs[i];
  if (start >= end) {
    return;
  }
  while (i < j) {
    while (i < j && strcmp(strs[j], base) >= 0) {
      j--;
    }
    strs[i] = strs[j];
    while (i < j && strcmp(strs[i], base) <= 0) {
      i++;
    }
    strs[j] = strs[i];
  }
  strs[i] = base;
  _str_sort(strs, start, i - 1);
  _str_sort(strs, j + 1, end);
}

static char *_str_cat(const char *strs[], uni_s32 str_num) {
  uni_s32 i;
  uni_s32 total_len = 0;
  char *cat_str;
  for (i = 0; i < str_num; i++) {
    total_len += uni_strlen(strs[i]);
  }
  cat_str = (char *)uni_malloc(total_len + 1);
  if (NULL == cat_str) {
    LOGE(CLOUD_UTILS_TAG, "memory malloc failed");
    return NULL;
  }
  *cat_str = '\0';
  for (i = 0; i < str_num; i++) {
    uni_strcat(cat_str, strs[i]);
  }
  return cat_str;
}

Result BuildSignature(const char *params[], uni_s32 param_num, char *result) {
  char *in_str = NULL;
  _str_sort(params, 0, param_num - 1);
  in_str = _str_cat(params, param_num);
  if (NULL == in_str) {
    return E_FAILED;
  }
  SHA1DigestStr(in_str, result);
  uni_free(in_str);
  return E_OK;
}
