#ifndef HTTP_SERVER_HTTP_REQUEST_HANDLER_H_
#define HTTP_SERVER_HTTP_REQUEST_HANDLER_H_

#include <string>
using namespace std;

class HttpRequestHandler {
    private:
    string name;
    public:
    HttpRequestHandler(const char* name) ;
    string getName(); 
    virtual int handle(string& path, string& request, string&response) = 0;
};

#endif