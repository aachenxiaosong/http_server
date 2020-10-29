#ifndef APP_UP_SULINK_HTTP_SULINK_TIME_SYNC_HPP_
#define APP_UP_SULINK_HTTP_SULINK_TIME_SYNC_HPP_

#include <string>
using namespace std;

class SulinkTimeSync {
private:
    long T1;
    long T2;
    long T3;
    long T4;
    int parseResult(const string& result);
    int calculateLocalTime();
public:
    SulinkTimeSync()
    {
        T1 = T2 = T3 = T4 = 0;
    }
    int request();
};

#endif  //  APP_UP_SULINK_HTTP_SULINK_TIME_SYNC_HPP_