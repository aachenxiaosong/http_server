#include "ITcpDataHandler.hpp"

ITcpDataHandler :: ITcpDataHandler(const char* name) {
    this->mName = name;
}

ITcpDataHandler :: ~ITcpDataHandler() {
    
}

string ITcpDataHandler :: getName() {
    return mName;
}