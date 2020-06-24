#ifndef HTTP_SERVER_HTTP_SERVER_H_
#define HTTP_SERVER_HTTP_SERVER_H_

#include "IHttpRequestHandler.hpp"

int http_server_start() ;
int http_server_add_handler(IHttpRequestHandler *handler);
int http_server_clear_handler();


#endif  //  HTTP_SERVER_HTTP_SERVER_H_