#include "LiftCtrlMqPacker.hpp"
#include "UniSerialization.hpp"
#include "UniLog.hpp"

#define LIFT_CTRL_PACKER_TAG "lift_ctrl_packer"

LiftCtrlMessageWechatCtrl* LiftCtrlMqPacker :: unpackWechatCtrl(const string& raw_data)
{
    LiftCtrlMessageWechatCtrl request = unisound::UniSerialization<LiftCtrlMessageWechatCtrl>::deseri(raw_data);
    return new LiftCtrlMessageWechatCtrl(request);
}

LiftCtrlMessageWechatStatus* LiftCtrlMqPacker :: unpackWechatStatus(const string& raw_data)
{
    LiftCtrlMessageWechatStatus request = unisound::UniSerialization<LiftCtrlMessageWechatStatus>::deseri(raw_data);
    return new LiftCtrlMessageWechatStatus(request);
}

string* LiftCtrlMqPacker :: packWechatCtrlAck(const LiftCtrlMessageWechatCtrlAck &message)
{
    string response = unisound::UniSerialization<LiftCtrlMessageWechatCtrlAck>::seri(message);
    return new string(response);
}

string* LiftCtrlMqPacker :: packWechatStatusAck(const LiftCtrlMessageWechatStatusAck &message)
{
    string response = unisound::UniSerialization<LiftCtrlMessageWechatStatusAck>::seri(message);
    return new string(response);
}

LiftCtrlMessageReq* LiftCtrlMqPacker :: unpack(MqTopicType topic, const string& raw_data)
{
    Message message = unisound::UniSerialization<Message>::deseri(raw_data);
    switch (message.type())
    {
        case MSG_LIFT_CTRL_WECHAT_CTRL: {
            return unpackWechatCtrl(raw_data);
        }
        case MSG_LIFT_CTRL_WECHAT_STATUS: {
            return unpackWechatStatus(raw_data);
        }
    }
    LOGW(LIFT_CTRL_PACKER_TAG, "unsupport mq msg type %d", message.type());
    return NULL;
}

string* LiftCtrlMqPacker :: pack(const LiftCtrlMessageRsp &message)
{
    switch (message.type()) {
        case MSG_LIFT_CTRL_WECHAT_CTRL_ACK: {
            return packWechatCtrlAck(dynamic_cast<const LiftCtrlMessageWechatCtrlAck&>(message));
        }
        case MSG_LIFT_CTRL_WECHAT_STATUS_ACK: {
            return packWechatStatusAck(dynamic_cast<const LiftCtrlMessageWechatStatusAck&>(message));
        }
    }
    LOGT(LIFT_CTRL_PACKER_TAG, "unsupport msg type %d", message.type());
    return NULL;
}
