#include "ITcpRequestHandler.hpp"

ITcpRequestHandler :: ITcpRequestHandler(const char* name) {
    this->mName = name;
}

ITcpRequestHandler :: ~ITcpRequestHandler() {
    
}

string ITcpRequestHandler :: getName() {
    return mName;
}