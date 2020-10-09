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
    void add(char *ip, uint16_t port, void *socket);
    void del(char *ip);
    void del(char *ip, uint16_t port);
    void del(void *socket);
    void *get(char *ip, uint16_t port);
    void *get(char *ip);
};

#endif  //  SDK_TCP_TCP_CONN_MGR_TCP_CONN_MGR_HPP_