#ifndef SDK_TCP_TCP_CONN_MGR_TCP_CONN_HPP_
#define SDK_TCP_TCP_CONN_MGR_TCP_CONN_HPP_

#include "ITcpReceiver.hpp"
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
    vector <ITcpReceiver *> mReceivers;
    ITcpPacker *mPacker;
    RwLock mReceiverLock;

public:
    TcpConn(string &ip, uint16_t port, void *socket, vector<ITcpReceiver *> &receivers, ITcpPacker *packer);
    ~TcpConn();
    void setTag(string &tag);
    string& getTag();
    string& getIp();
    uint16_t getPort();
    void* getSocket();
    //给tcp server调用
    int onRecv(const char *data, int len);
    //给app调用
    int send(const char *data, int len);
};

#endif  //  SDK_TCP_TCP_CONN_MGR_TCP_CONN_HPP_