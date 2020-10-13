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
 * Description : uni_auth_http.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.03.31
 *
 **************************************************************************/
#ifndef APP_RIGHT_COMMON_AUTH_UNI_AUTH_HTTP_H_
#define APP_RIGHT_COMMON_AUTH_UNI_AUTH_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

#define MAX_TOKEN_LEN  128

typedef struct {
  int  valid_time;
  char token[MAX_TOKEN_LEN];
} Token;

Token* HttpGetToken();
void   HttpGetTokenFree(Token *token);

int    HttpGetTokenCacheUpdate();

#ifdef __cplusplus
}
#endif
#endif  //  APP_RIGHT_COMMON_AUTH_UNI_AUTH_HTTP_H_
