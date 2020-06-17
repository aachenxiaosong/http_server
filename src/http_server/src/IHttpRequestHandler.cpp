#include "IHttpRequestHandler.hpp"

IHttpRequestHandler :: IHttpRequestHandler(const char* name) {
    this->mName = name;
}

IHttpRequestHandler :: ~IHttpRequestHandler() {
    
}

string IHttpRequestHandler :: getName() {
    return mName;
}