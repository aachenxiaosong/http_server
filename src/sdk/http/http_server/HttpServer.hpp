#ifndef  SDK_HTTP_HTTP_SERVER_HTTP_SERVER_HPP_
#define  SDK_HTTP_HTTP_SERVER_HTTP_SERVER_HPP_

#include "IHttpRequestHandler.hpp"

int http_server_start();
int http_server_add_handler(IHttpRequestHandler *handler);
int http_server_del_handler(IHttpRequestHandler *handler);
int http_server_clear_handler();


#endif  //   SDK_HTTP_HTTP_SERVER_HTTP_SERVER_HPP_