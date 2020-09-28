#ifndef  SDK_HTTP_HTTP_SERVER_I_HTTP_REQUEST_HANDLER_HPP_
#define  SDK_HTTP_HTTP_SERVER_I_HTTP_REQUEST_HANDLER_HPP_

#include <string>
using namespace std;

class IHttpRequestHandler {
    private:
    string mName;
    public:
    IHttpRequestHandler(const char* name);
    virtual ~IHttpRequestHandler();
    string getName();
    virtual int handle(string& path, string& request, string&response) = 0;
};

#endif  //   SDK_HTTP_HTTP_SERVER_I_HTTP_REQUEST_HANDLER_HPP_