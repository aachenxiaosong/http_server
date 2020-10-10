#ifndef  SDK_TCP_TCP_HANDLE_I_TCP_RECEIVER_HPP_
#define  SDK_TCP_TCP_HANDLE_I_TCP_RECEIVER_HPP_

#include "STcpConn.hpp"

#include <string>
using namespace std;

class ITcpReceiver {
    private:
    string mName;
    public:
    ITcpReceiver(const char* name);
    virtual ~ITcpReceiver();
    string getName();
    //return value: 0=receive ok, others=not mine data
    virtual int onRecv(STcpConn *conn, char *data, int len) = 0;
};

#endif  //   SDK_TCP_TCP_HANDLE_I_TCP_RECEIVER_HPP_