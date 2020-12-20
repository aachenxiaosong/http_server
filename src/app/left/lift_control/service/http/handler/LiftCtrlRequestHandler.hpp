#pragma once

#include "ILiftCtrlHttpMessageHandler.hpp"
#include "IHttpRequestHandler.hpp"
#include "LiftCtrlRequestPacker.hpp"
#include <string>
#include <vector>

using namespace std;

class LiftCtrlRequestHandler : public IHttpRequestHandler {
private:
    vector<ILiftCtrlHttpMessageHandler *> mMessageHandlers;
    LiftCtrlRequestPacker mPacker;
public:
    LiftCtrlRequestHandler(const char *name) : IHttpRequestHandler(name) {}
    ~LiftCtrlRequestHandler() {
        mMessageHandlers.clear();
    }
    int addMessageHandler(ILiftCtrlHttpMessageHandler *handler);
    int handle(const string& path, const string& request, string& response);
private:
    string failureResponse(int ret_code, string msg);
};
