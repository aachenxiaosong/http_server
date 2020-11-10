#include "SulinkRecvLiftInfoHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SulinkTraceid.hpp"
#include "MqttClient.hpp"
#include "UniUtil.hpp"
#include "configurable_info.h"
#include "uni_log.h"

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
    long timestamp = UniUtil::timestampMs();
    ack.traceId(SulinkTraceid::build(to_string(timestamp)));
    ack.payloadVersion(SULINK_PAYLOAD_VERSION);
    ack.brand(SULINK_BRAND);
    ack.timestamp(timestamp);
    ack.reqId(msg.reqId());
    ack.ackReqId(UniUtil::uuid());
    ack.deviceCode(UniUtil::deviceCode());
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