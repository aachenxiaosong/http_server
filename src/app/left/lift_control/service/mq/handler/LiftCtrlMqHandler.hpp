#pragma once

#include "ILiftCtrlMqMessageHandler.hpp"
#include "LiftCtrlMqPacker.hpp"
#include "Mq.hpp"
#include <string>
#include <vector>

using namespace std;

class LiftCtrlMqHandler {
private:
    string mName;
    vector<ILiftCtrlMqMessageHandler *> mMessageHandlers;
    LiftCtrlMqPacker mPacker;
public:
    LiftCtrlMqHandler(const char *name) {
        mName = name;
    }
    ~LiftCtrlMqHandler() {
        mMessageHandlers.clear();
    }
    int addMessageHandler(ILiftCtrlMqMessageHandler *handler);
    //message handler do ack by itself
    int handle(MqTopicType topic, const string& request);
private:
    string failureResponse(int ret_code, string msg);
};
