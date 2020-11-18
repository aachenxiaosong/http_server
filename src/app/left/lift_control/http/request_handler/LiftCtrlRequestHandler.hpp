#ifndef LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_LIFT_CTRL_REQUEST_HANDLER_H_
#define LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_LIFT_CTRL_REQUEST_HANDLER_H_

#include "ILiftCtrlMessageHandler.hpp"
#include "IHttpRequestHandler.hpp"
#include "LiftCtrlPacker.hpp"
#include <string>
#include <vector>

using namespace std;

class LiftCtrlRequestHandler : public IHttpRequestHandler {
private:
    vector<ILiftCtrlMessageHandler *> mMessageHandlers;
    LiftCtrlPacker mPacker;
public:
    LiftCtrlRequestHandler() : IHttpRequestHandler("lift_ctrl_http_handler") {}
    ~LiftCtrlRequestHandler() {
        mMessageHandlers.clear();
    }
    int addMessageHandler(ILiftCtrlMessageHandler *handler);
    int handle(const string& path, const string& request, string& response);
private:
    string failureResponse(int ret_code, string msg);
};

#endif  //  LEFT_LIFT_CONTROL_HTTP_REQUEST_HANDLER_LIFT_CTRL_REQUEST_HANDLER_H_