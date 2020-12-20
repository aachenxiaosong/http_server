#pragma once

#include "LiftCtrlMessage.hpp"
#include "Mq.hpp"

class LiftCtrlMqPacker {
private:
    LiftCtrlMessageWechatCtrl* unpackWechatCtrl(const string& raw_data);
    LiftCtrlMessageWechatStatus* unpackWechatStatus(const string& raw_data);
    string* packWechatCtrlAck(const LiftCtrlMessageWechatCtrlAck &message);
    string* packWechatStatusAck(const LiftCtrlMessageWechatStatusAck &message);

public:
    LiftCtrlMqPacker() = default;
    ~LiftCtrlMqPacker() = default;
    //return message need to be released by app
    LiftCtrlMessageReq* unpack(MqTopicType topic, const string& raw_data);
    //return string need to be released by app
    string* pack(const LiftCtrlMessageRsp &message);
};
