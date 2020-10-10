#include "TcpServerTest.hpp"
#include "TcpServer.hpp"
#include "uni_log.h"
#include <string.h>

#define TCP_REQ_HANDLER_TAG getName().c_str()

//static TcpServer *server = new TcpServer("test_tcp_server", 8888);
//static TcpHandle *handle;

class TcpTest
{
private:
    TcpHandle *handle;
    TcpServer *server;
    class TcpReceiver1 : public ITcpReceiver
    {
    private:
        TcpTest *parent;
    public:
        TcpReceiver1(TcpTest *parent) : ITcpReceiver("test_tcp_receiver1") {
            this->parent = parent;
        }
        ~TcpReceiver1() {}
        int onRecv(STcpConn *conn, char *data, int len)
        {
            LOGT(TCP_REQ_HANDLER_TAG, "tcp data: %s, len %d", data, len);
            if (strstr(data, "123")) {
                parent->handle->send(conn, (const char *)"response from test receiver1", strlen("response from test receiver1") + 1);
                return 0;
            }
            return -1;
        }
    } *receiver1;
    class TcpReceiver2 : public ITcpReceiver
    {
    private:
        TcpTest *parent;
    public:
        TcpReceiver2(TcpTest *parent) : ITcpReceiver("test_tcp_receiver2") {
            this->parent = parent;
        }
        ~TcpReceiver2() {}
        int onRecv(STcpConn *conn, char *data, int len)
        {
            LOGT(TCP_REQ_HANDLER_TAG, "tcp data: %s, len %d", data, len);
            if (strstr(data, "abc")) {
                parent->handle->send(conn, (const char *)"response from test receiver2", strlen("response from test receiver2") + 1);
                return 0;
            }
            return -1;
        }
    } *receiver2;
    
    
public:
    TcpTest() {
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

void TcpServerTest() {
    TcpTest test;
    test.test();
}

//static TcpReceiver1 *receiver = new TcpReceiver1();

