#ifndef SDK_TCP_TCP_CONN_MGR_S_TCP_CONN_HPP_
#define SDK_TCP_TCP_CONN_MGR_S_TCP_CONN_HPP_

#include <stdint.h>
#include <string>

using namespace std;

struct STcpConn {
    string tag;//给上层app用于标识conn
    string ip;
    uint16_t port;
    void *socket;
};

#endif  //  SDK_TCP_TCP_CONN_MGR_S_TCP_CONN_HPP_