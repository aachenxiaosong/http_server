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


int SulinkClient :: send(const Message& message) {
    if (!mIsConnected) {
        return -1;
    }
    return mMqttClient->send(message);
}

#define SULINK_CLIENT_TAG1 client->mName.c_str()
void SulinkClient :: linkTask(void *arg) {
    SulinkClient *client = (SulinkClient *)arg;
    while (client->mDeviceRegister.request() != 0) {
        sleep(10);
    }
    client->mMqttClient = new MqttClient(client->mName.c_str(),
                                         client->mDeviceRegister.getServerIp(),
                                         client->mDeviceRegister.getServerPort(),
                                         client->mDeviceRegister.getPubTopics(),
                                         client->mDeviceRegister.getSubTopics());
    client->mMqttClient->setPacker(&client->mPacker);
    client->mSendDeviceInfoHandler.setConn(client->mMqttClient);
    client->mRecvPassRuleInfoHandler.setConn(client->mMqttClient);
    client->mMqttClient->addHandler(&client->mSendDeviceInfoHandler);
    client->mMqttClient->addHandler(&client->mRecvPassRuleInfoHandler);
    client->mMqttClient->connect();
    client->mIsConnected = true;
    LOGT(SULINK_CLIENT_TAG1, "connecting to server...");
}
