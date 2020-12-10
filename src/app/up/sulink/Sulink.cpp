#include "Sulink.hpp"
#include "SulinkLiftInitData.hpp"
#include <iostream>

int Sulink :: init()
{
    if (mHb.startHbTask() != 0) {
        return -1;
    }
    if (mMqttClient.start() != 0) {
        return -1;
    }
    SulinkLiftInitData::loadFromConfig();
    return 0;
}