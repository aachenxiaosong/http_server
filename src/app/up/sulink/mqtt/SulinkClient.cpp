#include "SulinkClient.hpp"
#include "uni_log.h"

#define SULINK_CLIENT_TAG mName.c_str()

SulinkClient :: SulinkClient() {
    mName = "sulink_mqtt_client";
    mMqttClient = NULL;
    mIsConnected = false;
    mLinkThread = new thread(linkTask, this);
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


int SulinkClient :: onRecvMessage(const Message& message) {
    if (mMqttClient != NULL) {
        return mMqttClient->onRecv(message);
    }
    return -1;
}

#define SULINK_CLIENT_TAG1 client->mName.c_str()
void SulinkClient :: linkTask(void *arg) {
    SulinkClient *client = (SulinkClient *)arg;
    while (client->mDeviceRegister.request() != 0) {
        sleep(10);
    }
    client->mMqttClient = new MqttClient(client->mName.c_str(),
                                         client->mDeviceRegister.getMqttClientParam());
    client->mMqttClient->setPacker(&client->mPacker);

    client->mSendDeviceInfoHandler.setConn(client->mMqttClient);
    client->mSendPassRecordHandler.setConn(client->mMqttClient);
    client->mRecvPassRuleHandler.setConn(client->mMqttClient);
    client->mRecvLiftInfoHandler.setConn(client->mMqttClient);

    client->mMqttClient->addHandler(&client->mSendDeviceInfoHandler);
    client->mMqttClient->addHandler(&client->mSendPassRecordHandler);
    client->mMqttClient->addHandler(&client->mRecvPassRuleHandler);
    client->mMqttClient->addHandler(&client->mRecvLiftInfoHandler);
    
    client->mMqttClient->connect();
    client->mIsConnected = true;
    LOGT(SULINK_CLIENT_TAG1, "connecting to server...");
}
