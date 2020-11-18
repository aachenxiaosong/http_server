#ifndef APP_UP_SULINK_SULINK_HPP_
#define APP_UP_SULINK_SULINK_HPP_

#include "SulinkHb.hpp"
#include "SulinkClient.hpp"

class Sulink {
private:
    SulinkHb mHb;
    SulinkClient mMqttClient;
public:
    Sulink() = default;
    ~Sulink() = default;
    int init();
};

#endif  //  APP_UP_SULINK_SULINK_HPP_