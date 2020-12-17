#pragma once

#include "LiftCtrlMessage.hpp"
#include <string>

using namespace std;

class ILiftCtrlMessageHandler {
protected:
    string mName;
public:
    ILiftCtrlMessageHandler(const char* name) : mName(name) {
    }
    virtual ~ILiftCtrlMessageHandler() {}
    const string& getName() {
        return mName;
    }
    //return message need to be released by caller
    virtual LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request) = 0;
};
