#ifndef APP_UP_SULINK_HTTP_SULINK_DEVICE_REGISTER_HPP_
#define APP_UP_SULINK_HTTP_SULINK_DEVICE_REGISTER_HPP_

#include "MqttClientParam.hpp"

class SulinkDeviceRegister {
private:
    MqttClientParam mParam;
public:
    SulinkDeviceRegister() = default;
    ~SulinkDeviceRegister() = default;
    const MqttClientParam& getMqttClientParam() {return mParam;}
private:
    int parseResult(const string& result);
public:
    int request();
};

#endif  //  APP_UP_SULINK_HTTP_SULINK_DEVICE_REGISTER_HPP_
