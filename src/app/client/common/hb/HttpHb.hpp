#ifndef APP_CLIENT_COMMON_HB_HTTP_HB_H_
#define APP_CLIENT_COMMON_HB_HTTP_HB_H_

#include "HttpClient.hpp"
#include <thread>

class HttpHb
{
private:
    static const char *mLogTag;
    static thread *mThread;
    static void hbReportTask();

public:
    static int init();
};

#endif  //  APP_CLIENT_COMMON_HB_HTTP_HB_H_