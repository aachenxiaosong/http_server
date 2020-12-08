
#ifndef UTILS_UNI_UTIL_INC_UNI_UTIL_H_
#define UTILS_UNI_UTIL_INC_UNI_UTIL_H_

#include "uni_uuid.h"
#include "uni_time_tick.h"
#include "uni_md5sum.h"
#include <string>

using namespace std;

class UniUtil {
public:
    /* UUID */
    static string uuid() {
        char id[UUID_LEN + 1];
        GetUuid(id);
        return id;
    }
    /* timestamp */
    static long timestampMs() {
        return uni_get_now_msec();
    }
    /* md5 */
    static string md5(const string& text) {
        uint8_t digest[MD5SUM_LEN];
        size_t initial_len = text.length();
        Md5sum((const uint8_t *)text.data(), initial_len, digest);
        char md5[MD5SUM_LEN * 2 + 1] = {0};
        const char *dict = "0123456789abcdef";
        int index = -1;
        uint8_t c;
        char *o = md5;
        while (++index < MD5SUM_LEN)
        {
            c = digest[index];
            *o++ = dict[(c >> 4) & 0x0F];
            *o++ = dict[c & 0x0F];
        }
        return md5;
    }
};

#endif  // UTILS_UNI_UTIL_INC_UNI_UTIL_H_
