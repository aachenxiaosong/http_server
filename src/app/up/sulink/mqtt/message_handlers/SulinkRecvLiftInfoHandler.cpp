#include "SulinkRecvLiftInfoHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkConfigData.hpp"
#include "MqttClient.hpp"
#include "UniDeviceInfo.hpp"
#include "UniUtil.hpp"
#include "configurable_info.h"
#include "UniLog.hpp"

#define SULINK_RECV_LIFT_INFO_TAG getName().c_str()
SulinkRecvLiftInfoHandler :: SulinkRecvLiftInfoHandler() : IMqttMessageHandler("sulink_recv_Lift_handler") {}

SulinkRecvLiftInfoHandler :: ~SulinkRecvLiftInfoHandler() {}

int SulinkRecvLiftInfoHandler :: handle(const Message &message) {
    if (mConn == NULL)
    {
        LOGE(SULINK_RECV_LIFT_INFO_TAG, "mqtt client is null");
        return -1;
    }
    if (message.type() != MSG_SULINK_RECV_LIFT_INFO) {
        return -1;
    }
    const SulinkMessageRecvLiftInfo& msg = dynamic_cast<const SulinkMessageRecvLiftInfo&>(message);
    SulinkMessageRecvLiftInfoAck ack;
    long timestamp = unisound::UniUtil::timestampMs();
    ack.topic("pub/lift");
    ack.traceId(SulinkTraceid::build(to_string(timestamp)));
    ack.payloadVersion(SulinkConfigData::getPayloadVersion());
    ack.brand(SulinkConfigData::getBrand());
    ack.timestamp(timestamp);
    ack.reqId(msg.reqId());
    ack.ackReqId(unisound::UniUtil::uuid());
    ack.deviceCode(unisound::UniDeviceInfo::getUdid());
    ack.code(msg.errCode());
    ack.message(msg.errMessage());
    if (msg.errCode() == 0) {
        SulinkLiftInitData::updateInfo(msg);
    } else {
        LOGE(SULINK_RECV_LIFT_INFO_TAG, "skip update info for unpack error %s", msg.errMessage().c_str());
    }
    mConn->send(ack);
    return 0;
}
