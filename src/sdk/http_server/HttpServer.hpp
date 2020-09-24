#ifndef  SDK_HTTP_SERVER_HTTP_SERVER_H_
#define  SDK_HTTP_SERVER_HTTP_SERVER_H_

#include "IHttpRequestHandler.hpp"

int http_server_start() ;
int http_server_add_handler(IHttpRequestHandler *handler);
int http_server_clear_handler();


#endif  //   SDK_HTTP_SERVER_HTTP_SERVER_H_