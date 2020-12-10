#pragma once

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
    LiftCtrlRequestHandler(const char *name) : IHttpRequestHandler(name) {}
    ~LiftCtrlRequestHandler() {
        mMessageHandlers.clear();
    }
    int addMessageHandler(ILiftCtrlMessageHandler *handler);
    int handle(const string& path, const string& request, string& response);
private:
    string failureResponse(int ret_code, string msg);
};
