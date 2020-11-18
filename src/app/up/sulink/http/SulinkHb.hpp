#ifndef APP_UP_SULINK_HTTP_SULINK_HB_HPP_
#define APP_UP_SULINK_HTTP_SULINK_HB_HPP_

#include <thread>

#define HB_INTERVAL 60
using namespace std;

class SulinkHb {
private:
    thread *mHbThread;
    bool mIsRunning;
public:
    SulinkHb();
    ~SulinkHb();
    int startHbTask();
    int request();
private:
    static void hbTask(void *arg);
};

#endif  //  APP_UP_SULINK_HTTP_SULINK_HB_HPP_