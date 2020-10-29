#ifndef SDK_MESSAGE_MESSAGES_SULINK_MESSAGE_HPP_
#define SDK_MESSAGE_MESSAGES_SULINK_MESSAGE_HPP_

#include "IMqttMessage.hpp"
#include <string>

using namespace std;
//基础消息
class SulinkMessage : public IMqttMessage {
    MEMBER(string, trace_id)
    MEMBER(int, payload_version)
    MEMBER(string, brand)
    MEMBER(long, timestamp)
    MEMBER(string, method)
public:
    SulinkMessage(MessageType type) : IMqttMessage(type) {
        payload_version(1);
    }
    virtual ~SulinkMessage() {}
};

//心跳消息
class SulinkMessageSendDeviceInfo : public SulinkMessage {
    MEMBER(string, app_key)
    MEMBER(string, device_sn)
    //up_time就是unix时间戳
    MEMBER(long, up_time)
    MEMBER(string, app_version)
    MEMBER(string, device_type)
public:
    SulinkMessageSendDeviceInfo() : SulinkMessage(MSG_SULINK_SEND_DEVICE_INFO) {
        method("device-info");
        up_time(0);
    }
    ~SulinkMessageSendDeviceInfo() {}
};

/*class SulinkMessageSendHb : public SulinkMessage {
    MEMBER(string, device_code)
    MEMBER(long, up_time)
    MEMBER(double, cpu)
    MEMBER(double, memory)
    MEMBER(double, used_storage)
    MEMBER(double, total_storage)
    MEMBER(string, version_number)
public:
    SulinkMessageSendHb() : SulinkMessage(MSG_SULINK_SEND_HB) {
        method("");
        up_time(0);
        cpu(0);
        memory(0);
        used_storage(0);
        total_storage(0);
    }
    ~SulinkMessageSendHb() {}
};*/

class SulinkMessageRecvPassRuleInfo : public SulinkMessage {
    MEMBER(string, device_code)
    MEMBER(string, req_id)
    //TODO
public:
    SulinkMessageRecvPassRuleInfo() : SulinkMessage(MSG_SULINK_RECV_PASS_RULE_INFO) {
        method("pass-rule-info");
    }
    ~SulinkMessageRecvPassRuleInfo() {}
};

class SulinkMessageRecvPassRuleAck : public SulinkMessage {
    MEMBER(string, device_code)
    MEMBER(string, req_id)
    MEMBER(string, ack_req_id)
    //TODO
public:
    SulinkMessageRecvPassRuleAck() : SulinkMessage(MSG_SULINK_RECV_PASS_RULE_ACK) {
        method("pass-rule-ack");
    }
    ~SulinkMessageRecvPassRuleAck() {}
};
//通行记录上报
class SulinkMessageSendPassRecord : public SulinkMessage {
    MEMBER(string, device_code)
    MEMBER(string, code)
    MEMBER(string, ext_data)
    MEMBER(string, pass_photo)
    MEMBER(int, pass_result)
    MEMBER(string, pass_time)
    MEMBER(string, pass_type)
    MEMBER(string, person_id)
    MEMBER(string, person_name)
    MEMBER(string, person_temp)
    MEMBER(int, person_type)
    MEMBER(string, req_id)
public:
    SulinkMessageSendPassRecord() : SulinkMessage(MSG_SULINK_SEND_PASS_RECORD) {
        method("pass-record");
    }
    ~SulinkMessageSendPassRecord() {}
};
class SulinkMessageSendPassRecordAck : public SulinkMessage {
public:
    SulinkMessageSendPassRecordAck() : SulinkMessage(MSG_SULINK_SEND_PASS_RECORD_ACK) {
        method("pass-record-ack");
    }
    ~SulinkMessageSendPassRecordAck() {}
};

#endif  //  SDK_MESSAGE_MESSAGES_SULINK_MESSAGE_HPP_