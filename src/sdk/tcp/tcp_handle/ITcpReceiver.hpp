#ifndef  SDK_TCP_TCP_HANDLE_I_TCP_RECEIVER_HPP_
#define  SDK_TCP_TCP_HANDLE_I_TCP_RECEIVER_HPP_

#include <string>
using namespace std;

class TcpConn;

class ITcpReceiver {
    protected:
    string mName;
    TcpConn *mConn;
    public:
    ITcpReceiver(const char* name);
    virtual ~ITcpReceiver();
    string getName();
    void setConn(TcpConn *conn);
    //return value: 0=receive ok, others=not mine data
    //接收数据后需要ACK,跟主动发送走一样的路径,即通过tag找到conn,然后发送
    //recv到的数据通过event发到上层,由上层决定是否发送
    virtual int onRecv(const char *data, int len) = 0;
    virtual ITcpReceiver *copy() = 0;
};

#endif  //   SDK_TCP_TCP_HANDLE_I_TCP_RECEIVER_HPP_