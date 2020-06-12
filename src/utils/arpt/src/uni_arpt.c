/**********************************************************************
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
 **********************************************************************
 *
 * Description : Uniapp uni_arpt.c
 * Author      : wangzhicheng@unisound.com 
 * Date        : 2017.11.14
 *
 **********************************************************************/
#include "uni_arpt.h"
#include "uni_log.h"

#define ARPT_TAG "arpt"
#define MAX_ARPT_PATH 32

static struct {
  uni_bool is_open;
  uni_bool is_more;
  uni_s32  fd;
} g_arpt = {FALSE, FALSE, 0};


int ArptInit(const char *path) {
  char filename[MAX_ARPT_PATH + 16];
  if (g_arpt.is_open) {
    LOGE(ARPT_TAG, "arpt is already inited!!");
    return -1;
  }
  if (NULL == path || uni_strlen(path) >= MAX_ARPT_PATH) {
    LOGE(ARPT_TAG, "invalid path(%p)!!", path);
    return -1;
  }
  uni_snprintf(filename, sizeof(filename), "%s/debug_open", path);
  if (0 == access(filename, F_OK)) {
    g_arpt.is_open = TRUE;
  }
  if (g_arpt.is_open) {
    uni_snprintf(filename, sizeof(filename), "%s/debug_more", path);
    if (0 == access(filename, F_OK)) {
      g_arpt.is_more = TRUE;
    }
    uni_snprintf(filename, sizeof(filename), "%s/asr_log", path);
    g_arpt.fd = uni_open(filename, O_RDWR|O_CREAT|O_APPEND, 0644);
    if (0 >= g_arpt.fd) {
      LOGE(ARPT_TAG, "arpt fd open failed!!");
      return -1;
    }
  }
  return 0;
}

void ArptFinal(void) {
  if (g_arpt.fd) {
    uni_close(g_arpt.fd);
  }
  uni_memset(&g_arpt, 0, sizeof(g_arpt));
}

int ArptWrite(const char *content, uni_bool is_more) {
  if (g_arpt.is_open && g_arpt.fd) {
    if(!is_more || g_arpt.is_more) {
      uni_write(g_arpt.fd, content, uni_strlen(content));
      return 0;
    }
  }
  return -1;
}
