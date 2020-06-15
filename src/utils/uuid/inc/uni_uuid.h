/**************************************************************************
 * Description : uni_uuid.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.06.19
 *
 **************************************************************************/
#ifndef UTILS_UUID_INC_UNI_UUID_H_
#define UTILS_UUID_INC_UNI_UUID_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UUID_LEN 32
int GetUuid(char *uuid);

#ifdef __cplusplus
}
#endif
#endif  // UTILS_UUID_INC_UNI_UUID_H_
