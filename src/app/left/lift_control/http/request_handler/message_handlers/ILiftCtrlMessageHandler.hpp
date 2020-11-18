#ifndef LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_I_LIFT_CTRL_MESSAGE_HANDLER_H_
#define LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_I_LIFT_CTRL_MESSAGE_HANDLER_H_

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

#endif  //  LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_MESSAGE_HANDLERS_I_LIFT_CTRL_MESSAGE_HANDLER_H_
