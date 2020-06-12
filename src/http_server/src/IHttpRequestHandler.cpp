#include "IHttpRequestHandler.hpp"

IHttpRequestHandler :: IHttpRequestHandler(const char* name) {
    this->mName = name;
}

string IHttpRequestHandler :: getName() {
    return mName;
}