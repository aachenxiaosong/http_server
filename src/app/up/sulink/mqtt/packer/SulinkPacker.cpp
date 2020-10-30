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
    jheader.Add("traceId", message.traceId());
    jheader.Add("payloadVersion", message.payloadVersion());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jdevice;
    jdevice.Add("appKey", message.appKey());
    jdevice.Add("deviceSn", message.deviceSn());
    jdevice.Add("upTime", message.upTime());
    jdevice.Add("appVersion", message.appVersion());
    jdevice.Add("deviceType", message.deviceType());
    CJsonObject jpayload;
    jpayload.AddEmptySubArray("devices");
    jpayload["devices"].Add(jdevice);
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

string* SulinkPacker :: packSendPassRecord(const SulinkMessageSendPassRecord& message) {
    CJsonObject jheader;
    jheader.Add("traceId", message.traceId());
    jheader.Add("payloadVersion", message.payloadVersion());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jrecord;
    jrecord.Add("code", message.code());
    jrecord.Add("extData", message.extData());
    jrecord.Add("passPhoto", message.passPhoto());
    jrecord.Add("passResult", message.passResult());
    jrecord.Add("passTime", message.passTime());
    jrecord.Add("passType", message.passType());
    jrecord.Add("personId", message.personId());
    jrecord.Add("personName", message.personName());
    jrecord.Add("personTemp", message.personTemp());
    jrecord.Add("personType", message.personType());
    CJsonObject jpayload;
    jpayload.AddEmptySubArray("passRecords");
    jpayload["passRecords"].Add(jrecord);
    jpayload.Add("deviceCode", message.deviceCode());
    jpayload.Add("reqId", message.reqId());
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

SulinkMessageRecvPassRule* SulinkPacker :: unpackRecvPassRule(const string& raw_data) {
    //TODO
    return NULL;
}

SulinkMessageSendPassRecordAck* SulinkPacker :: unpackSendPassRecordAck(const string& raw_data) {
    CJsonObject jack(raw_data);
    CJsonObject jheader;
    jack.Get("header", jheader);
    string svalue;
    int ivalue;
    long lvalue;
    SulinkMessageSendPassRecordAck* ack = new SulinkMessageSendPassRecordAck();
    jack.Get("traceId", svalue);
    ack->traceId(svalue);
    jack.Get("payloadVersion", ivalue);
    ack->payloadVersion(ivalue);
    jack.Get("timestamp", lvalue);
    ack->timestamp(lvalue);
    CJsonObject jpayload;
    if (true == jack.Get("payload", jpayload)) {
        if (true == jpayload.Get("code", ivalue)) {
            ack->code(ivalue);
        }
    }
    return ack;
}


IMqttMessage* SulinkPacker :: unpack(const string& raw_data) {
    if (packCheck(raw_data) != 0) {
        return NULL;
    }
    string message_type;
    CJsonObject(raw_data)["header"].Get("method", message_type);
    if (message_type.compare("pass-rule-info") == 0) {
        return unpackRecvPassRule(raw_data);
    } else if (message_type.compare("pass-record-ack") == 0) {
        return unpackSendPassRecordAck(raw_data);
    }
    return NULL;
}

string* SulinkPacker :: pack(const IMqttMessage &message) {
    string *data = NULL;
    switch (message.type()) {
        case MSG_SULINK_SEND_DEVICE_INFO:
            data = packSendDeviceInfo(dynamic_cast<const SulinkMessageSendDeviceInfo&>(message));
            break;
        case MSG_SULINK_SEND_PASS_RECORD:
            data = packSendPassRecord(dynamic_cast<const SulinkMessageSendPassRecord&>(message));
            break;
        default:
            break;
    }
    return data;
}