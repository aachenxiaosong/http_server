#include "SulinkRecvLiftInfoHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkConfigData.hpp"
#include "UniDeviceInfo.hpp"
#include "UniUtil.hpp"
#include "configurable_info.h"
#include "UniLog.hpp"

#define SULINK_RECV_LIFT_INFO_TAG getName().c_str()
SulinkRecvLiftInfoHandler :: SulinkRecvLiftInfoHandler() : ISulinkMessageHandler("sulink_recv_lift_info_handler") {}

SulinkRecvLiftInfoHandler :: ~SulinkRecvLiftInfoHandler() {}

Message * SulinkRecvLiftInfoHandler :: handle(const Message &message) {
    if (message.type() != MSG_SULINK_RECV_LIFT_INFO) {
        return NULL;
    }
    const SulinkMessageRecvLiftInfo& msg = dynamic_cast<const SulinkMessageRecvLiftInfo&>(message);
    SulinkMessageRecvLiftInfoAck *ack = new SulinkMessageRecvLiftInfoAck();
    long timestamp = unisound::UniUtil::timestampMs();
    ack->topic("pub/lift");
    ack->traceId(SulinkTraceid::build(to_string(timestamp)));
    ack->payloadVersion(SulinkConfigData::getPayloadVersion());
    ack->brand(SulinkConfigData::getBrand());
    ack->timestamp(timestamp);
    ack->reqId(msg.reqId());
    ack->ackReqId(unisound::UniUtil::uuid());
    ack->deviceCode(unisound::UniDeviceInfo::getUdid());
    ack->code(msg.errCode());
    ack->message(msg.errMessage());
    if (msg.errCode() == 0) {
        SulinkLiftInitData::updateInfo(msg);
    } else {
        LOGE(SULINK_RECV_LIFT_INFO_TAG, "skip update info for unpack error %s", msg.errMessage().c_str());
    }
    return ack;
}
