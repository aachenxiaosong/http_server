#include "SulinkSendDeviceInfoHandler.hpp"
#include "SulinkMessage.hpp"
#include "MqttClient.hpp"
#include "uni_log.h"

#define SULINK_SEND_DEVICE_INFO_TAG getName().c_str()

SulinkSendDeviceInfoHandler :: SulinkSendDeviceInfoHandler() : IMqttMessageHandler("sulink_send_device_info_handler") {}

SulinkSendDeviceInfoHandler :: ~SulinkSendDeviceInfoHandler() {}

int SulinkSendDeviceInfoHandler :: handle(const Message &message) {
    if (mConn == NULL)
    {
        LOGE(SULINK_SEND_DEVICE_INFO_TAG, "mqtt client is null");
        return -1;
    }
    if (message.type() == MSG_SULINK_SEND_DEVICE_INFO)
    {
        LOGT(SULINK_SEND_DEVICE_INFO_TAG, "message MSG_SULINK_SEND_DEVICE_INFO is sent");
        return mConn->send(message);
    }
    return -1;
}