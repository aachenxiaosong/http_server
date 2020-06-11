#include "http_request_handler.h"

HttpRequestHandler::HttpRequestHandler(string& name) {
    this->name = name;
}

string HttpRequestHandler::getName() {
    return name;
}