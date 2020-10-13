#include "HttpHb.hpp"
#include "uni_hb_http.h"
#include "uni_log.h"

const char* HttpHb::mLogTag = "http_hb";
thread* HttpHb::mThread = NULL;

void HttpHb::hbReportTask() {
    while (1) {
        HttpHbReport();
        sleep(HB_PERIOD_SEC);
    }
}

int HttpHb::init() {
    mThread = new thread(hbReportTask);
    if (mThread == NULL) {
        LOGE(mLogTag, "HttpHb create hb report thread failed");
        return -1;
    }
    return 0;
}
