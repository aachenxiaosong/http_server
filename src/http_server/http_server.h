#ifndef HTTP_SERVER_HTTP_SERVER_H_
#define HTTP_SERVER_HTTP_SERVER_H_

#include "http_request_handler.h"

int http_server_start() ;
int http_server_add_handler(HttpRequestHandler *handler);
int http_server_remove_handler(string *handler_name);


#endif  //  HTTPSERVER_HTTPSERVER_H_