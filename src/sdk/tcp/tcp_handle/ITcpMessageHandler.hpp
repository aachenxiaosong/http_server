#ifndef  SDK_TCP_TCP_HANDLE_I_TCP_MESSAGE_HANDLER_HPP_
#define  SDK_TCP_TCP_HANDLE_I_TCP_MESSAGE_HANDLER_HPP_

#include "IMessageHandler.hpp"
#include <string>

using namespace std;

class TcpConn;

class ITcpMessageHandler : public IMessageHandler {
    protected:
    TcpConn *mConn;
    public:
    ITcpMessageHandler(const char* name) : IMessageHandler(name) {}
    virtual ~ITcpMessageHandler() {}
    void setConn(TcpConn *conn) { mConn = conn; }
    virtual int handle(const Message &message) = 0;
};

#endif  //   SDK_TCP_TCP_HANDLE_I_TCP_MESSAGE_HANDLER_HPP_