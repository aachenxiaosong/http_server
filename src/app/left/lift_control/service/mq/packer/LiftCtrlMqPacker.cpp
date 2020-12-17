#include "LiftCtrlMqPacker.hpp"
#include "UniSerialization.hpp"
#include "UniLog.hpp"

#define LIFT_CTRL_PACKER_TAG "lift_ctrl_packer"

LiftCtrlMessageWechatCtrl* LiftCtrlMqPacker :: unpackWechatCtrl(const string& raw_data)
{
    LiftCtrlMessageWechatCtrl request = unisound::UniSerialization<LiftCtrlMessageWechatCtrl>::deseri(raw_data);
    return new LiftCtrlMessageWechatCtrl(request);
}

string* LiftCtrlMqPacker :: packRsp(const LiftCtrlMessageRsp &message)
{
    string response = unisound::UniSerialization<LiftCtrlMessageRsp>::seri(message);
    return new string(response);
}

string* LiftCtrlMqPacker :: packWechatCtrlAck(const LiftCtrlMessageWechatCtrlAck &message)
{
    string response = unisound::UniSerialization<LiftCtrlMessageWechatCtrlAck>::seri(message);
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
        case MSG_LIFT_CTRL_RSP: {
            return packRsp(dynamic_cast<const LiftCtrlMessageRsp&>(message));
        }
    }
    LOGT(LIFT_CTRL_PACKER_TAG, "unsupport msg type %d", message.type());
    return NULL;
}
