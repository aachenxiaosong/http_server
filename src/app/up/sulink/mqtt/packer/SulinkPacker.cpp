#include "SulinkPacker.hpp"
#include "SulinkMessage.hpp"
#include "SulinkTraceid.hpp"
#include "UniUtil.hpp"
#include "uni_log.h"

#define SULINK_PACKER_TAG getName().c_str()

SulinkPacker :: SulinkPacker() : IMqttPacker("sulink_packer") {}

SulinkPacker :: ~SulinkPacker() {}


string* SulinkPacker :: packSendDeviceInfo(const SulinkMessageSendDeviceInfo& message) {
    CJsonObject jheader;
    //long timestamp = UniUtil::timestampMsL();
    //jheader.Add("traceId", SulinkTraceid::build(to_string(timestamp)));
    jheader.Add("traceId", message.trace_id());
    jheader.Add("payloadVersion", message.payload_version());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jdevice;
    jdevice.Add("appKey", message.app_key());
    jdevice.Add("deviceSn", message.device_sn());
    jdevice.Add("upTime", message.up_time());
    jdevice.Add("appVersion", message.app_version());
    jdevice.Add("deviceType", message.device_type());
    CJsonObject jpayload;
    jpayload.AddEmptySubArray("devices");
    jpayload["devices"].Add(jdevice);
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

int SulinkPacker :: packCheck(const string& raw_data) {
    CJsonObject jmessage(raw_data);
    CJsonObject jheader;
    CJsonObject jvalue;
    long lvalue;
    if (true != jmessage.Get("header", jheader)) {
        LOGE(SULINK_PACKER_TAG, "check header failed");
        return -1;
    }
    if (true != jheader.Get("traceId", jvalue) ||
        true != jheader.Get("payloadVersion", lvalue) ||
        true != jheader.Get("brand", jvalue) ||
        true != jheader.Get("timestamp", lvalue) ||
        true != jheader.Get("method", jvalue)) {
        LOGE(SULINK_PACKER_TAG, "check header content failed");
        return -1;
    }
    return 0;
}

SulinkMessageRecvPassRuleInfo* SulinkPacker :: unpackRecvPassRuleInfo(const string& raw_data) {
    //TODO
    return NULL;
}

IMqttMessage* SulinkPacker :: unpack(const string& raw_data) {
    if (packCheck(raw_data) != 0) {
        return NULL;
    }
    string message_type;
    CJsonObject(raw_data)["header"].Get("method", message_type);
    if (message_type.compare("pass-rule-info") == 0) {
        return unpackRecvPassRuleInfo(raw_data);
    }
    CJsonObject jheader;
    return NULL;
}

string* SulinkPacker :: pack(const IMqttMessage &message) {
    string *data = NULL;
    switch (message.type()) {
        case MSG_SULINK_SEND_DEVICE_INFO:
            data = packSendDeviceInfo(dynamic_cast<const SulinkMessageSendDeviceInfo&>(message));
            break;
        default:
            break;
    }
    return data;
}