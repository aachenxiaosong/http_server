/* *************************************************************************
 * Copyright (C) 2019-2019  Unisound
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
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * *************************************************************************
 *
 *  Description : uni_base64.h
 *  Author      : guozhenkun@unisound.com
 *  Date        : 2019.09.09
 *
 * *************************************************************************/

#ifndef _UTILS_ENCRYPT_INC_UNI_BASE64_H_
#define _UTILS_ENCRYPT_INC_UNI_BASE64_H_
#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief encode `in_str` to base64 string
   *
   * @param[in] in_str input string to encode
   * @param[in] in_len input string length
   * @param[out] out_str encoded string
   *
   * @return 0 if success, -1 otherwise.
   */
int base64_encode(char *in_str, int in_len, char **out_str);


/**
 * @brief Do Not use this API, havn't test yet.
 *
 * @param in_str
 * @param in_len
 * @param out_str
 *
 * @return 
 */
int base64_decode(char *in_str, int in_len, char *out_str);
#ifdef __cplusplus
}
#endif
#endif
