#include "SulinkSendDeviceInfoHandler.hpp"
#include "SulinkMessage.hpp"
#include "MqttClient.hpp"


SulinkSendDeviceInfoHandler :: SulinkSendDeviceInfoHandler() : IMqttMessageHandler("sulink_send_device_info_handler") {}

SulinkSendDeviceInfoHandler :: ~SulinkSendDeviceInfoHandler() {}

int SulinkSendDeviceInfoHandler :: handle(const Message &message) {
    const SulinkMessageSendDeviceInfo& msg = dynamic_cast<const SulinkMessageSendDeviceInfo&>(message);
    if (mConn != NULL) {
        return mConn->send(msg);
    }
    return -1;
}