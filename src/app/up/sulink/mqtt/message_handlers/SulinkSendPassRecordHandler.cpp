#include "SulinkSendPassRecordHandler.hpp"
#include "SulinkMessage.hpp"
#include "MqttClient.hpp"
#include "uni_log.h"
#include <unistd.h>

#define SULINK_SEND_PASS_RECORD_TAG getName().c_str()

SulinkSendPassRecordHandler :: SulinkSendPassRecordHandler() : IMqttMessageHandler("sulink_send_device_info_handler") {
    mAckReceived = false;
}

SulinkSendPassRecordHandler :: ~SulinkSendPassRecordHandler() {}

int SulinkSendPassRecordHandler :: handle(const Message &message) {
    if (mConn == NULL)
    {
        LOGT(SULINK_SEND_PASS_RECORD_TAG, "mqtt client is null");
        return -1;
    }
    if (message.type() == MSG_SULINK_SEND_PASS_RECORD)
    {
        mAckReceived = false;
        int wait_time = 1000 * 3; //3s超时
        if (0 == mConn->send(message))
        {
            while (mAckReceived == 0 && wait_time > 0)
            {
                usleep(1000 * 10);
                wait_time -= 10;
            }
            if (mAckReceived == 0) {
                LOGE(SULINK_SEND_PASS_RECORD_TAG, "%s sent timeout(3s)", message.type());
            } else {
                LOGT(SULINK_SEND_PASS_RECORD_TAG, "%s sent OK", message.type());
            }
        } else {
            LOGE(SULINK_SEND_PASS_RECORD_TAG, "%s sent failed", message.type());
        }
        return 0;
    } else if (message.type() == MSG_SULINK_SEND_PASS_RECORD_ACK) {
        LOGT(SULINK_SEND_PASS_RECORD_TAG, "%s received", message.type());
        return 0;
    }
    return -1;
}