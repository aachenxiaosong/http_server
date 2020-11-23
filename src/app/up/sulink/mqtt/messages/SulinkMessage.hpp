#ifndef APP_UP_SULINK_MQTT_MESSAGES_SULINK_MESSAGE_HPP_
#define APP_UP_SULINK_MQTT_MESSAGES_SULINK_MESSAGE_HPP_

#include "IMqttMessage.hpp"
#include <string>

#define SPACE_TYPE_HOME     "home"
#define SPACE_TYPE_FLOOR    "floor"
#define SPACE_TYPE_UNIT     "unit"
#define SPACE_TYPE_BUILDING "building"

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

//梯控初始化
class LiftInfoSpace {
    MEMBER(string, id)
    MEMBER(string, spaceName)
    MEMBER(string, pid)
    MEMBER(string, elevatorSpaceType)
    MEMBER(string, elevatorSpaceNo)
    //空间对应群控器ID列表（以,号分割）
    MEMBER(string, clusterControllerIds)
    MEMBER(string, floorLocation)
    MEMBER(long, timestamp)
public:
    LiftInfoSpace() {
        timestamp(0);
    }
};
class LiftInfoCluster {
    MEMBER(string, controllerIp)
    MEMBER(string, controllerId)
};
class LiftInfoBrand {
    MEMBER(string, brandName)
    //厂商code(1-旺龙 2-日立)
    MEMBER(string, brandCode)
    MEMBER(string, intranetUrl)
    MEMBER(string, appId)
    MEMBER(string, appSecret)
    MEMBER(string, licence)
    //通信协议(默认http)
    MEMBER(string, schema)
    PMEMBER(vector<LiftInfoCluster>, clusterControllers)
};
class LiftInfoAccessDevice {
    MEMBER(string, deviceCode)
    //工作模式(1.世茂梯控板(轿厢外),2.工控机模式(轿厢内),3.工控机模式(轿厢外))
    MEMBER(int, workMode)
    //楼层信息(以，号分割)
    MEMBER(string, floorMap)
    //公共楼层(以，号分割)
    MEMBER(string, publicFloor)
    MEMBER(string, curFloor)
    //电梯ID(工作模式2 -工控机模式(轿厢内) 必传)
    MEMBER(string, liftId)
    //大厅号（日立电梯 工作模式3 - 工控机模式(轿厢外)必传
    MEMBER(string, hallNo)
    MEMBER(long, timestamp)
public:
    LiftInfoAccessDevice() {
        workMode(0);
        timestamp(0);
    }
};
class SulinkMessageRecvLiftInfo : public SulinkMessage {
    MEMBER(string, deviceCode)
    PMEMBER(vector<LiftInfoSpace>, spaces)
    PMEMBER(LiftInfoBrand, brandInfo)
    PMEMBER(vector<LiftInfoAccessDevice>, accessDevices)
    MEMBER(long, timestamp)
    MEMBER(string, extData)
    MEMBER(string, reqId)
    //错误信息,用于将解析失败的情况报给handler
    MEMBER(int, errCode)
    MEMBER(string, errMessage)
public:
    SulinkMessageRecvLiftInfo() : SulinkMessage(MSG_SULINK_RECV_LIFT_INFO) {
        method("device-ubox-lift-info");
        timestamp(0);
        errCode(0);
    }
    ~SulinkMessageRecvLiftInfo() {}
};
class SulinkMessageRecvLiftInfoAck : public SulinkMessage {
    MEMBER(string, deviceCode)
    //规则下发结果(0-成功 1-失败)
    MEMBER(int, code)
    //信息字段（错误信息等）
    MEMBER(string, message)
    MEMBER(string, reqId)
    MEMBER(string, ackReqId)
    public:
    SulinkMessageRecvLiftInfoAck() : SulinkMessage(MSG_SULINK_RECV_LIFT_INFO_ACK) {
        method("device-ubox-lift-info-ack");
        code(0);
    }
    ~SulinkMessageRecvLiftInfoAck() {}
};
#endif  //  APP_UP_SULINK_MQTT_MESSAGES_SULINK_MESSAGE_HPP_
