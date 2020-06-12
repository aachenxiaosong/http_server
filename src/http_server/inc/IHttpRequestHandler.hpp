#ifndef HTTP_SERVER_I_HTTP_REQUEST_HANDLER_H_
#define HTTP_SERVER_I_HTTP_REQUEST_HANDLER_H_

#include <string>
using namespace std;

class IHttpRequestHandler {
    private:
    string mName;
    public:
    IHttpRequestHandler(const char* name) ;
    string getName(); 
    virtual int handle(string& path, string& request, string&response) = 0;
};

#endif