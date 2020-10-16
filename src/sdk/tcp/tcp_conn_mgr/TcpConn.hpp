#ifndef SDK_TCP_TCP_CONN_MGR_TCP_CONN_HPP_
#define SDK_TCP_TCP_CONN_MGR_TCP_CONN_HPP_

#include "MessageDispatcher.hpp"
#include "ITcpMessageHandler.hpp"
#include "ITcpPacker.hpp"
#include "RwLock.hpp"
#include <stdint.h>
#include <string>


using namespace std;

class TcpConn {
private:
    string mName;
    string mTag;//给上层app用于标识conn
    string mIp;
    uint16_t mPort;
    void *mSocket;

private:
    vector <ITcpMessageHandler *> mHandlers;
    ITcpPacker *mPacker;
    RwLock mReceiverLock;

public:
    TcpConn(const string &ip, uint16_t port, void *socket, vector<ITcpMessageHandler *> &handlers, ITcpPacker *packer);
    ~TcpConn();
    void setTag(const string &tag);
    string& getTag();
    string& getIp();
    uint16_t getPort();
    void* getSocket();
    //给tcp server调用
    int onRecv(const char *data, int len);
    //给app调用
    int send(const char *data, int len);
    int onRecv(const Message& message);
    int send(const Message& message);
};

#endif  //  SDK_TCP_TCP_CONN_MGR_TCP_CONN_HPP_