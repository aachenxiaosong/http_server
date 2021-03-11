#pragma once

#include "ISulinkPacker.hpp"
#include "ISulinkMessageHandler.hpp"
#include "IHttpRequestHandler.hpp"
#include <string>



using namespace std;

class SulinkRequestHandler : public IHttpRequestHandler {

private:
    ISulinkPacker *mPacker;
    vector <ISulinkMessageHandler *> mHandlers;
public:
    SulinkRequestHandler(const char *name) : IHttpRequestHandler(name) {}
    ~SulinkRequestHandler() {
        mHandlers.clear();
    }
    int setPacker(ISulinkPacker *packer);
    int addMessageHandler(ISulinkMessageHandler *handler);
    int handle(const string& path, const string& request, string& response);
};


