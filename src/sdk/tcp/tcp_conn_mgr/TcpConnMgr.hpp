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
    void add(const char *ip, uint16_t port, void *socket);
    void del(const char *ip);
    void del(const char *ip, uint16_t port);
    void del(void *socket);
    STcpConn *get(const char *ip, uint16_t port);
    STcpConn *get(const char *ip);
    STcpConn *get(void *socket);
};

#endif  //  SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_