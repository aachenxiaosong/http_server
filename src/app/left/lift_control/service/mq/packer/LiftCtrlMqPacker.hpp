#pragma once

#include "LiftCtrlMessage.hpp"
#include "Mq.hpp"

class LiftCtrlMqPacker {
private:
    LiftCtrlMessageWechatCtrl* unpackWechatCtrl(const string& raw_data);
    string* packRsp(const LiftCtrlMessageRsp &message);
    string* packWechatCtrlAck(const LiftCtrlMessageWechatCtrlAck &message);

public:
    LiftCtrlMqPacker() = default;
    ~LiftCtrlMqPacker() = default;
    //return message need to be released by app
    LiftCtrlMessageReq* unpack(MqTopicType topic, const string& raw_data);
    //return string need to be released by app
    string* pack(const LiftCtrlMessageRsp &message);
};
