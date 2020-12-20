#include "SulinkRecvLiftCtrlHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkConfigData.hpp"
#include "LiftCtrlMessage.hpp"
#include "UniSerialization.hpp"
#include "MqttClient.hpp"
#include "UniDeviceInfo.hpp"
#include "UniUtil.hpp"
#include "configurable_info.h"
#include "UniLog.hpp"

#define SULINK_RECV_LIFT_CTRL_TAG getName().c_str()
SulinkRecvLiftCtrlHandler :: SulinkRecvLiftCtrlHandler() : IMqttMessageHandler("sulink_recv_Lift_ctrl_handler"), mMq("sulink_recv_lift_ctrl_handler_mq") {}

SulinkRecvLiftCtrlHandler :: ~SulinkRecvLiftCtrlHandler() {}

int SulinkRecvLiftCtrlHandler :: handle(const Message &message) {
    if (mConn == NULL)
    {
        LOGE(SULINK_RECV_LIFT_CTRL_TAG, "mqtt client is null");
        return -1;
    }
    if (message.type() != MSG_SULINK_RECV_LIFT_CTRL) {
        return -1;
    }
    const SulinkMessageRecvLiftCtrl& msg = dynamic_cast<const SulinkMessageRecvLiftCtrl&>(message);
    LiftCtrlMessageWechatCtrl req;
    req.reqId(msg.reqId());
    req.deviceCode(msg.deviceCode());
    req.homeId(msg.homeId());
    req.fromFloor(msg.fromFloor());
    req.toFloor(msg.toFloor());
    string content = unisound::UniSerialization<LiftCtrlMessageWechatCtrl>::seri(req);
    MqData data(MQ_TOPIC_LIFT_CTRL_WECHAT, (void *)content.c_str(), content.length() + 1);
    mMq.send(data);
    return 0;
}
