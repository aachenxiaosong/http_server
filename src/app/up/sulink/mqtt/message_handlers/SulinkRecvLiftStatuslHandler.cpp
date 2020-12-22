#include "SulinkRecvLiftStatusHandler.hpp"
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

#define SULINK_RECV_LIFT_STATUS_TAG getName().c_str()
SulinkRecvLiftStatusHandler :: SulinkRecvLiftStatusHandler() : IMqttMessageHandler("sulink_recv_lift_status_handler"), mMq("sulink_recv_lift_status_handler_mq") {}

SulinkRecvLiftStatusHandler :: ~SulinkRecvLiftStatusHandler() {}

int SulinkRecvLiftStatusHandler :: handle(const Message &message) {
    if (mConn == NULL)
    {
        LOGE(SULINK_RECV_LIFT_STATUS_TAG, "mqtt client is null");
        return -1;
    }
    if (message.type() != MSG_SULINK_RECV_LIFT_STATUS) {
        return -1;
    }
    const SulinkMessageRecvLiftStatus& msg = dynamic_cast<const SulinkMessageRecvLiftStatus&>(message);
    LiftCtrlMessageWechatStatus req;
    req.retcode(msg.errCode());
    req.msg(msg.errMessage());
    req.reqId(msg.reqId());
    req.homeId(msg.homeId());
    req.elevatorId(msg.elevatorId());
    string content = unisound::UniSerialization<LiftCtrlMessageWechatStatus>::seri(req);
    MqData data(MQ_TOPIC_LIFT_CTRL_WECHAT, (void *)content.c_str(), content.length() + 1);
    mMq.send(data);
    return 0;
}
