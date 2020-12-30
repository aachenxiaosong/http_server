#pragma once

#include "LiftCtrlMessage.hpp"
#include <string>
#include <vector>

using namespace std;

class ILiftCtrlHttpMessageHandler {
protected:
    string mName;
public:
    ILiftCtrlHttpMessageHandler(const char* name) : mName(name) {
    }
    virtual ~ILiftCtrlHttpMessageHandler() {}
    const string& getName() {
        return mName;
    }
    //return message need to be released by caller
    virtual LiftCtrlMessageRsp* handle(const LiftCtrlMessageReq &request) = 0;
};
