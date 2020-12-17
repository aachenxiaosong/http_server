#pragma once

#include "ILiftCtrlMessageHandler.hpp"
#include "LiftCtrlMqPacker.hpp"
#include "Mq.hpp"
#include <string>
#include <vector>

using namespace std;

class LiftCtrlMqHandler {
private:
    string mName;
    vector<ILiftCtrlMessageHandler *> mMessageHandlers;
    LiftCtrlMqPacker mPacker;
public:
    LiftCtrlMqHandler(const char *name) {
        mName = name;
    }
    ~LiftCtrlMqHandler() {
        mMessageHandlers.clear();
    }
    int addMessageHandler(ILiftCtrlMessageHandler *handler);
    int handle(MqTopicType topic, const string& request, string& response);
private:
    string failureResponse(int ret_code, string msg);
};
