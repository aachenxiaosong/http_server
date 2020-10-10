#include "TcpServerTest.hpp"
#include "TcpServer.hpp"
#include "uni_log.h"
#include <string.h>

#define TCP_SERVER_TEST_TAG getName().c_str()

class ServerTest
{
private:
    TcpHandle *handle;
    TcpServer *server;
    class TcpReceiver1 : public ITcpReceiver
    {
    private:
        ServerTest *parent;
    public:
        TcpReceiver1(ServerTest *parent) : ITcpReceiver("tcp_server_receiver1") {
            this->parent = parent;
        }
        ~TcpReceiver1() {}
        int onRecv(STcpConn *conn, char *data, int len)
        {
            LOGT(TCP_SERVER_TEST_TAG, "tcp data: %s, len %d from connection %s:%d",
                 data, len, conn->ip, conn->port);
            if (strstr(data, "123")) {
                parent->handle->send(conn, (const char *)"response of 123 from server receiver1", strlen("response of 123 from server receiver1") + 1);
                return 0;
            }
            return -1;
        }
    } *receiver1;
    class TcpReceiver2 : public ITcpReceiver
    {
    private:
        ServerTest *parent;
    public:
        TcpReceiver2(ServerTest *parent) : ITcpReceiver("tcp_server_receiver2") {
            this->parent = parent;
        }
        ~TcpReceiver2() {}
        int onRecv(STcpConn *conn, char *data, int len)
        {
            LOGT(TCP_SERVER_TEST_TAG, "tcp data: %s, len %d from connection %s:%d",
                 data, len, conn->ip, conn->port);
            if (strstr(data, "abc")) {
                parent->handle->send(conn, (const char *)"response of abc from server receiver2", strlen("response of abc from server receiver2") + 1);
                return 0;
            }
            return -1;
        }
    } *receiver2;
    
    
public:
    ServerTest() {
        server = new TcpServer("test_tcp_server", 8888);
        handle = server->getHandle();
        receiver1 = new TcpReceiver1(this);
        handle->addReceiver(receiver1);
        receiver2 = new TcpReceiver2(this);
        handle->addReceiver(receiver2);
    }

    void test()
    {
        server->listen();
    }
};

static ServerTest test;
void TcpServerTest() {
    test.test();
}

//static TcpReceiver1 *receiver = new TcpReceiver1();

