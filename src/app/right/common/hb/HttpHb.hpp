#ifndef APP_RIGHT_COMMON_HB_HTTP_HB_H_
#define APP_RIGHT_COMMON_HB_HTTP_HB_H_

#include "HttpClient.hpp"
#include <thread>


#define HB_PERIOD_SEC               (60)

class HttpHb
{
private:
    static const char *mLogTag;
    static thread *mThread;
    static void hbReportTask();

public:
    static int init();
};

#endif  //  APP_RIGHT_COMMON_HB_HTTP_HB_H_