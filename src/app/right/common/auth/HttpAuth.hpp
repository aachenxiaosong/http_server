#ifndef APP_RIGHT_COMMON_AUTH_HTTP_AUTH_H_
#define APP_RIGHT_COMMON_AUTH_HTTP_AUTH_H_

#include "HttpClient.hpp"
#include <thread>

#define TOKEN_REFRESH_PERIOD_SEC         (10)

class HttpAuth
{
private:
    static const char *mLogTag;
    static int mValidTime;
    static string mToken;
    static thread *mThread;
    static void tokenUpdateTask();

public:
    static int init();
    static string getToken();
};

#endif  //  APP_RIGHT_COMMON_AUTH_HTTP_AUTH_H_