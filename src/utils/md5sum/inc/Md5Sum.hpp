#ifndef UTILS_HTTP_INC_MD5_SUM_HPP_
#define UTILS_HTTP_INC_MD5_SUM_HPP_

#include "uni_md5sum.h"
#include <string>

using namespace std;

class Md5Sum {
public:
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

#endif  //  UTILS_HTTP_INC_MD5_SUM_HPP_
