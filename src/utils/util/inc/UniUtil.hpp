
#pragma once

#include "uni_iot.h"
#include <string>

using namespace std;

namespace unisound {
class UniUtil {
public:
    /* UUID */
    static string uuid() {
        const int uuid_len = 32;
        const char *c_charactors = "0123456789abcdefghijklmnopqrstuvwxyz";
        int chars_num = strlen(c_charactors);
        char id[uuid_len + 1];
        int i;
        for (i = 0; i < uuid_len; i++)
        {
            id[i] = c_charactors[rand() % chars_num];
        }
        id[i] = '\0';
        return id;
    }
    /* timestamp */
    /* timestamp from 1900  (ms) */
    static long timestampMs() {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        return ((int64_t)t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    /* md5 */
    static string md5(const string& text);
};
}
