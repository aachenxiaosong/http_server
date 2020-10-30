#ifndef APP_UP_SULINK_MQTT_PACKER_SULINK_PACKER_HPP_
#define APP_UP_SULINK_MQTT_PACKER_SULINK_PACKER_HPP_

#include "IMqttPacker.hpp"
#include "SulinkMessage.hpp"
#include "CJsonObject.hpp"

using namespace neb;

class SulinkPacker : public IMqttPacker {
private:
    string* packSendDeviceInfo(const SulinkMessageSendDeviceInfo& message);
    string* packSendPassRecord(const SulinkMessageSendPassRecord& message);
    int packCheck(const string& raw_data);
    SulinkMessageRecvPassRule* unpackRecvPassRule(const string& raw_data);
    SulinkMessageSendPassRecordAck* unpackSendPassRecordAck(const string& raw_data);
public:
    SulinkPacker();
    ~SulinkPacker();
    IMqttMessage* unpack(const string& raw_data);
    string* pack(const IMqttMessage &message);
};

#endif  //  APP_UP_SULINK_MQTT_PACKER_SULINK_PACKER_HPP_
