#include "SulinkClient.hpp"
#include "UniSerialization.hpp"
#include "LiftCtrlMessage.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkConfigData.hpp"
#include "UniLog.hpp"
#include "uni_iot.h"

#define SULINK_CLIENT_TAG mName.c_str()

SulinkClient :: SulinkClient() {
    mName = "sulink_mqtt_client";
    mMqttClient = NULL;
    mIsConnected = false;
    mMqThreadRunning = false;
}

SulinkClient :: ~SulinkClient() {
    if (mLinkThread != NULL)
    {
        mLinkThread->join();
        delete mLinkThread;
    }
    if (mMqttClient != NULL) {
        delete mMqttClient;
        mMqttClient = NULL;
    }
}

int SulinkClient :: start()
{
    mLinkThread = new thread(linkTask, this);
    if (mLinkThread == NULL) {
        LOGE(SULINK_CLIENT_TAG, "start sulink mqtt client link task failed");
        return -1;
    }
    vector<MqTopicType> topic_types;
    topic_types.push_back(MQ_TOPIC_SULINK_LIFT_CTRL);
    mMq = new Mq("sulink_lift_ctrl_mq", topic_types);
    mMqThreadRunning = false;
    mMqThread = new thread(mqRecvTask, this);
    return 0;
}

int SulinkClient :: send(const Message& message) {
    if (mMqttClient != NULL) {
        return mMqttClient->send(message);
    }
    return -1;
}

#define SULINK_CLIENT_TAG1 me->mName.c_str()
void SulinkClient :: linkTask(void *arg) {
    SulinkClient *me = (SulinkClient *)arg;
    while (me->mDeviceRegister.request() != 0) {
        sleep(10);
    }
    me->mMqttClient = new MqttClient(me->mName.c_str(),
                                         me->mDeviceRegister.getMqttClientParam());
    me->mMqttClient->setPacker(&me->mPacker);

    me->mSendDeviceInfoHandler.setConn(me->mMqttClient);
    me->mSendPassRecordHandler.setConn(me->mMqttClient);
    me->mRecvPassRuleHandler.setConn(me->mMqttClient);
    me->mRecvLiftInfoHandler.setConn(me->mMqttClient);
    me->mRecvLiftCtrlHandler.setConn(me->mMqttClient);

    me->mMqttClient->addHandler(&me->mSendDeviceInfoHandler);
    me->mMqttClient->addHandler(&me->mSendPassRecordHandler);
    me->mMqttClient->addHandler(&me->mRecvPassRuleHandler);
    me->mMqttClient->addHandler(&me->mRecvLiftInfoHandler);
    me->mMqttClient->addHandler(&me->mRecvLiftCtrlHandler);
    
    me->mMqttClient->connect();
    me->mIsConnected = true;
    LOGT(SULINK_CLIENT_TAG1, "connecting to server...");
}

void SulinkClient :: mqRecvTask(void *arg) {
    SulinkClient *me = (SulinkClient *)arg;
    me->mMqThreadRunning = true;
    while (me->mMqThreadRunning) {
        MqData data = me->mMq->recv();
        Message msg = unisound::UniSerialization<Message>::deseri((const char*)data.content());
        MessageType msg_type = msg.type();
        LOGT(SULINK_CLIENT_TAG1, "recv msg, topic %d msg_type %d", data.topic_type(), msg_type);
        if (data.topic_type() == MQ_TOPIC_SULINK_LIFT_CTRL) {
            switch (msg_type) {
                case MSG_LIFT_CTRL_WECHAT_CTRL_ACK: {
                    LiftCtrlMessageWechatCtrlAck lift_ctrl_ack = unisound::UniSerialization<LiftCtrlMessageWechatCtrlAck>::deseri((const char *)data.content());
                    SulinkMessageRecvLiftCtrlAck sulink_ack;
                    long timestamp = unisound::UniUtil::timestampMs();
                    sulink_ack.topic("pub/lift");
                    sulink_ack.traceId(SulinkTraceid::build(to_string(timestamp)));
                    sulink_ack.payloadVersion(SulinkConfigData::getPayloadVersion());
                    sulink_ack.brand(SulinkConfigData::getBrand());
                    sulink_ack.timestamp(timestamp);
                    sulink_ack.reqId(lift_ctrl_ack.reqId());
                    sulink_ack.ackReqId(unisound::UniUtil::uuid());
                    sulink_ack.deviceCode(unisound::UniDeviceInfo::getUdid());
                    sulink_ack.code(lift_ctrl_ack.retcode());
                    sulink_ack.message(lift_ctrl_ack.msg());
                    sulink_ack.curFloor(lift_ctrl_ack.curFloor());
                    sulink_ack.elevatorId(lift_ctrl_ack.elevatorId());
                    sulink_ack.upDown(lift_ctrl_ack.upDown());
                    me->send(sulink_ack);
                }
            }
            LOGT(SULINK_CLIENT_TAG1, "msg of topic %d is handled", data.topic_type());
        }
    }
}
