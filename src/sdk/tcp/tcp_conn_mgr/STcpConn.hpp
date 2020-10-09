#ifndef SDK_TCP_TCP_CONN_MGR_S_TCP_CONN_HPP_
#define SDK_TCP_TCP_CONN_MGR_S_TCP_CONN_HPP_

#include <stdint.h>

struct STcpConn {
    char ip[16];
    uint16_t port;
    void *socket;
};

#endif  //  SDK_TCP_TCP_CONN_MGR_S_TCP_CONN_HPP_