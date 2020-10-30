#ifndef APP_UP_SULINK_MQTT_MESSAGES_SULINK_MESSAGE_HPP_
#define APP_UP_SULINK_MQTT_MESSAGES_SULINK_MESSAGE_HPP_

#include "IMqttMessage.hpp"
#include <string>

using namespace std;
//基础消息
class SulinkMessage : public IMqttMessage {
    MEMBER(string, traceId)
    MEMBER(int, payloadVersion)
    MEMBER(string, brand)
    MEMBER(long, timestamp)
    MEMBER(string, method)
public:
    SulinkMessage(MessageType type) : IMqttMessage(type) {
        payloadVersion(1);
    }
    virtual ~SulinkMessage() {}
};

//设备信息上报
class SulinkMessageSendDeviceInfo : public SulinkMessage {
    MEMBER(string, appKey)
    MEMBER(string, deviceSn)
    //up_time就是unix时间戳
    MEMBER(long, upTime)
    MEMBER(string, appVersion)
    MEMBER(string, deviceType)
public:
    SulinkMessageSendDeviceInfo() : SulinkMessage(MSG_SULINK_SEND_DEVICE_INFO) {
        method("device-info");
        upTime(0);
    }
    ~SulinkMessageSendDeviceInfo() {}
};

//通行规则下发
class SulinkMessageRecvPassRule : public SulinkMessage {
    MEMBER(string, deviceCode)
    MEMBER(string, reqId)
    //TODO
public:
    SulinkMessageRecvPassRule() : SulinkMessage(MSG_SULINK_RECV_PASS_RULE) {
        method("pass-rule-info");
    }
    ~SulinkMessageRecvPassRule() {}
};

class SulinkMessageRecvPassRuleAck : public SulinkMessage {
    MEMBER(string, deviceCode)
    MEMBER(string, reqId)
    MEMBER(string, ackReqId)
    //TODO
public:
    SulinkMessageRecvPassRuleAck() : SulinkMessage(MSG_SULINK_RECV_PASS_RULE_ACK) {
        method("pass-rule-ack");
    }
    ~SulinkMessageRecvPassRuleAck() {}
};
//通行记录上报
class SulinkMessageSendPassRecord : public SulinkMessage {
    MEMBER(string, deviceCode)
    MEMBER(string, code)
    MEMBER(string, extData)
    MEMBER(string, passPhoto)
    MEMBER(int, passResult)
    MEMBER(string, passTime)
    MEMBER(string, passType)
    MEMBER(string, personId)
    MEMBER(string, personName)
    MEMBER(string, personTemp)
    MEMBER(int, personType)
    MEMBER(string, reqId)
public:
    SulinkMessageSendPassRecord() : SulinkMessage(MSG_SULINK_SEND_PASS_RECORD) {
        method("pass-record");
    }
    ~SulinkMessageSendPassRecord() {}
};
class SulinkMessageSendPassRecordAck : public SulinkMessage {
    MEMBER(int, code)
public:
    SulinkMessageSendPassRecordAck() : SulinkMessage(MSG_SULINK_SEND_PASS_RECORD_ACK) {
        method("pass-record-ack");
        code(-1);
    }
    ~SulinkMessageSendPassRecordAck() {}
};

#endif  //  APP_UP_SULINK_MQTT_MESSAGES_SULINK_MESSAGE_HPP_