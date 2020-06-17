#include "IMcMsgHandler.hpp"

IMcMsgHandler :: IMcMsgHandler(const char* name) {
    this->mName = name;
}

IMcMsgHandler :: ~IMcMsgHandler() {
}

string IMcMsgHandler :: getName() {
    return mName;
}