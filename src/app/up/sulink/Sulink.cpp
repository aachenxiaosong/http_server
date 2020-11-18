#include "Sulink.hpp"


int Sulink :: init()
{
    if (mHb.startHbTask() != 0) {
        return -1;
    }
    if (mMqttClient.start() != 0) {
        return -1;
    }
    return 0;
}