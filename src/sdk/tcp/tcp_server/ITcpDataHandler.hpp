#ifndef  SDK_TCP_TCP_SERVER_I_TCP_DATA_HANDLER_HPP_
#define  SDK_TCP_TCP_SERVER_I_TCP_DATA_HANDLER_HPP_

#include <string>
using namespace std;

class ITcpDataHandler {
    private:
    string mName;
    public:
    ITcpDataHandler(const char* name);
    virtual ~ITcpDataHandler();
    string getName();
    virtual int handle(char *request, int req_len, char *response, int *resp_len) = 0;
};

#endif  //   SDK_TCP_TCP_SERVER_I_TCP_DATA_HANDLER_HPP_