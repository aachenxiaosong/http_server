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
 * Description : uni_http.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2018.06.20
 *
 **************************************************************************/
#ifndef UTILS_HTTP_INC_UNI_HTTP_H_
#define UTILS_HTTP_INC_UNI_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "cJSON.h"

/* 20m */
#define GET_DEFAULT_TIMEOUT  20
#define POST_DEFAULT_TIMEOUT 20

int HttpGetWithHeadersTimeout(const char *url, const char *headers[][2],
                              int header_num, uni_u32 timeout,
                              char **result);

#define HttpGetWithHeaders(url, headers, header_num, result) \
        HttpGetWithHeadersTimeout(url, headers, header_num, \
                                  GET_DEFAULT_TIMEOUT, result)

#define HttpGetWithTimeout(url, timeout, result) \
        HttpGetWithHeadersTimeout(url, NULL, 0, timeout, result)

#define HttpGet(url, result) \
        HttpGetWithHeadersTimeout(url, NULL, 0, \
                                  GET_DEFAULT_TIMEOUT, result)


int HttpPostWithHeadersTimeout(const char *url, const char *content,
                               const char *headers[][2], int header_num,
                               uni_u32 timeout, char **result);

#define HttpPostWithHeaders(url, content, headers, header_num, result) \
        HttpPostWithHeadersTimeout(url, content, headers, header_num, \
                                   POST_DEFAULT_TIMEOUT, result)

#define HttpPostWithTimeout(url, content, timeout, result) \
        HttpPostWithHeadersTimeout(url, content, NULL, 0, timeout, result)

#define HttpPost(url, content, result) \
        HttpPostWithHeadersTimeout(url, content, NULL, 0, \
                                   POST_DEFAULT_TIMEOUT, result)

char *HttpUrlEncode(const char *str);

int HttpsGetWithHeadersTimeout(const char *url, const char *capath,
                               const char *headers[][2], int header_num,
                               uni_u32 timeout, char **result);

int HttpsPostWithHeadersTimeout(const char *url,const char *capath, const char *content,
                                const char *headers[][2], int header_num,
                                uni_u32 timeout, char **result);

int HttpUrlParse(const char *url, cJSON **out_dict);

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_HTTP_INC_UNI_HTTP_H_
