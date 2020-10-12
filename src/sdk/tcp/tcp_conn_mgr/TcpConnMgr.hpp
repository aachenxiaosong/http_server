#ifndef SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_
#define SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_

#include "STcpConn.hpp"
#include <vector>
#include <mutex>

using namespace std;

class TcpConnMgr {
private:
    vector<STcpConn *> mConns;
    mutex mLock;
public:
    TcpConnMgr() = default;
    ~TcpConnMgr();
    void add(string ip, uint16_t port, void *socket);
    void del(string ip);
    void del(string ip, uint16_t port);
    void del(void *socket);
    STcpConn *get(string ip, uint16_t port);
    STcpConn *get(string ip);
    STcpConn *get(void *socket);
    int setTag(STcpConn *conn, string tag);
    STcpConn *getByTag(string tag);

};

#endif  //  SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_