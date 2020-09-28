#ifndef  SDK_TCP_TCP_SERVER_I_TCP_REQUEST_HANDLER_HPP_
#define  SDK_TCP_TCP_SERVER_I_TCP_REQUEST_HANDLER_HPP_

#include <string>
using namespace std;

class ITcpRequestHandler {
    private:
    string mName;
    public:
    ITcpRequestHandler(const char* name);
    virtual ~ITcpRequestHandler();
    string getName();
    virtual int handle(void *conn, char *data, int len) = 0;
};

#endif  //   SDK_TCP_TCP_SERVER_I_TCP_REQUEST_HANDLER_HPP_