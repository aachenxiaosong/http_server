#include "SulinkSendPassRecordHandler.hpp"
#include "SulinkMessage.hpp"
#include "MqttClient.hpp"
#include "UniLog.hpp"
#include <unistd.h>

#define SULINK_SEND_PASS_RECORD_TAG getName().c_str()

SulinkSendPassRecordHandler :: SulinkSendPassRecordHandler() : IMqttMessageHandler("sulink_send_device_info_handler") {
    mAckReceived = false;
}

SulinkSendPassRecordHandler :: ~SulinkSendPassRecordHandler() {}

int SulinkSendPassRecordHandler :: handle(const Message &message) {
    if (mConn == NULL)
    {
        LOGE(SULINK_SEND_PASS_RECORD_TAG, "mqtt client is null");
        return -1;
    }
    if (message.type() != MSG_SULINK_SEND_PASS_RECORD && message.type() != MSG_SULINK_SEND_PASS_RECORD_ACK) {
        return -1;
    }

    if (message.type() == MSG_SULINK_SEND_PASS_RECORD)
    {
        mAckReceived = false;
        int wait_time = 1000 * 3; //3s超时
        if (0 == mConn->send(message))
        {
            while (mAckReceived == false && wait_time > 0)
            {
                usleep(1000 * 10);
                wait_time -= 10;
            }
            if (mAckReceived == false) {
                LOGE(SULINK_SEND_PASS_RECORD_TAG, "message MSG_SULINK_SEND_PASS_RECORD sent timeout(3s)");
            } else {
                LOGT(SULINK_SEND_PASS_RECORD_TAG, "message MSG_SULINK_SEND_PASS_RECORD sent OK");
            }
        } else {
            LOGE(SULINK_SEND_PASS_RECORD_TAG, "message MSG_SULINK_SEND_PASS_RECORD sent failed");
        }
        return 0;
    } else if (message.type() == MSG_SULINK_SEND_PASS_RECORD_ACK) {
        mAckReceived = true;
        const SulinkMessageSendPassRecordAck& msg = dynamic_cast<const SulinkMessageSendPassRecordAck&>(message);
        LOGT(SULINK_SEND_PASS_RECORD_TAG, "message MSG_SULINK_SEND_PASS_RECORD_ACK received, code = %d", msg.code());
        return 0;
    }
    return -1;
}