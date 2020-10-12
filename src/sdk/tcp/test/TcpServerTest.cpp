#include "TcpServerTest.hpp"
#include "TcpServer.hpp"
#include "uni_log.h"
#include <string.h>
#include <unistd.h>
#include <thread>


#define TCP_SERVER_TEST_TAG getName().c_str()

class ServerTest
{
private:
    TcpHandle *handle;
    TcpServer *server;
    std::thread *thread;

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
                 data, len, conn->ip.c_str(), conn->port);
            if (strstr(data, "123")) {
                parent->handle->send(conn, (const char *)"response of 123 from server receiver1", strlen("response of 123 from server receiver1") + 1);
                parent->server->getConnMgr()->setTag(conn, "123");
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
                 data, len, conn->ip.c_str(), conn->port);
            if (strstr(data, "abc")) {
                parent->handle->send(conn, (const char *)"response of abc from server receiver2", strlen("response of abc from server receiver2") + 1);
                parent->server->getConnMgr()->setTag(conn, "abc");
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

    static void heartbeatTask(void *arg) {
        ServerTest *test = (ServerTest *)arg;
        TcpHandle *handle = test->handle;
        TcpServer *server = test->server;
        STcpConn *conn;
        while (1) {
            sleep(1);
            conn = server->getConnMgr()->getByTag("123");
            if (conn)
            {
                handle->send(conn, "heartbeat", strlen("heartbeat") + 1);
                sleep(5);
            }
            conn = server->getConnMgr()->getByTag("abc");
            if (conn)
            {
                handle->send(conn, "heartbeat", strlen("heartbeat") + 1);
                sleep(5);
            }
        }
    }

    void test()
    {
        server->listen();
        thread = new std::thread(heartbeatTask, this);
    }
};

static ServerTest test;
void TcpServerTest() {
    test.test();
}

//static TcpReceiver1 *receiver = new TcpReceiver1();

