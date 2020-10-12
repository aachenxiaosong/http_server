#ifndef  SDK_TCP_TCP_HANDLE_TCP_HANDLE_HPP_
#define  SDK_TCP_TCP_HANDLE_TCP_HANDLE_HPP_

#include <vector>
#include "IProtocolPacker.hpp"
#include "ITcpReceiver.hpp"
#include "RwLock.hpp"
#include <string>
#include <mutex>
using namespace std;

#define MAX_TCP_PACK_LEN 4096
#define MAX_TCP_RESP_LEN 4096

class TcpHandle
{
private:
    vector <ITcpReceiver *> mReceivers;
    IProtocolPacker *mPacker;
    RwLock mReceiverLock;
public:
    TcpHandle() = default;
    virtual ~TcpHandle();
    //给app调用
    int setPacker(IProtocolPacker *packer);
    int addReceiver(ITcpReceiver *receiver);
    void delReceiver(ITcpReceiver *receiver);
    //给tcp server调用
    int onRecv(STcpConn *conn, const char *data, int len);
    //给app调用
    int send(STcpConn *conn, const char *data, int len);
};

#endif  //   SDK_TCP_TCP_HANDLE_TCP_HANDLE_HPP_