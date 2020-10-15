#ifndef SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_
#define SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_

#include "TcpConn.hpp"
#include <vector>
#include <mutex>

using namespace std;

class TcpConnMgr {
private:
    vector<TcpConn *> mConns;
    mutex mLock;
public:
    TcpConnMgr() = default;
    ~TcpConnMgr();
    void add(string ip, uint16_t port, void *socket, vector<ITcpMessageHandler *> &handlers, ITcpPacker *packer);
    void del(string ip);
    void del(string ip, uint16_t port);
    void del(void *socket);
    TcpConn *get(string ip, uint16_t port);
    TcpConn *get(string ip);
    TcpConn *get(void *socket);
    TcpConn *getByTag(string tag);

};

#endif  //  SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_