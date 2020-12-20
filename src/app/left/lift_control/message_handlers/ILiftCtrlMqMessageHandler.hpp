#pragma once

#include "LiftCtrlMessage.hpp"
#include "LiftCtrlMqPacker.hpp"
#include "Mq.hpp"
#include <string>

using namespace std;

class ILiftCtrlMqMessageHandler {
protected:
    string mName;
    Mq mMq;
    LiftCtrlMqPacker mPacker;
public:
    ILiftCtrlMqMessageHandler(const char* name) : mName(name), mMq(name) {
    }
    virtual ~ILiftCtrlMqMessageHandler() {}
    const string& getName() {
        return mName;
    }
    //return 0 if msg is handled
    virtual int handle(const LiftCtrlMessageReq &request) = 0;
};
