#include "http_request_handler.h"

HttpRequestHandler::HttpRequestHandler(const char* name) {
    this->name = name;
}

string HttpRequestHandler::getName() {
    return name;
}