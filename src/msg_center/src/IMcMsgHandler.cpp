#include "IMcMsgHandler.hpp"

IMcMsgHandler :: IMcMsgHandler(const char* name) {
    this->mName = name;
}

string IMcMsgHandler :: getName() {
    return mName;
}