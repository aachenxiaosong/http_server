#include "TcpClientTest.hpp"
#include "TcpClient.hpp"
#include "uni_log.h"
#include <string.h>
#include <unistd.h>

#define TCP_CLIENT_TEST_TAG getName().c_str()

class ClientTest
{
private:
    TcpHandle *handle;
    TcpClient *client;
    class TcpReceiver1 : public ITcpReceiver
    {
    private:
        ClientTest *parent;
    public:
        TcpReceiver1(ClientTest *parent) : ITcpReceiver("tcp_client_receiver1") {
            this->parent = parent;
        }
        ~TcpReceiver1() {}
        int onRecv(STcpConn *conn, char *data, int len)
        {
            LOGT(TCP_CLIENT_TEST_TAG, "tcp data: %s, len %d from connection %s:%d",
                 data, len, conn->ip, conn->port);
            if (strstr(data, "123")) {
                parent->handle->send(conn, (const char *)"response of ack from client receiver1", strlen("response of 123 from client receiver1") + 1);
                return 0;
            }
            return -1;
        }
    } *receiver1;
    class TcpReceiver2 : public ITcpReceiver
    {
    private:
        ClientTest *parent;
    public:
        TcpReceiver2(ClientTest *parent) : ITcpReceiver("tcp_client_receiver2") {
            this->parent = parent;
        }
        ~TcpReceiver2() {}
        int onRecv(STcpConn *conn, char *data, int len)
        {
            LOGT(TCP_CLIENT_TEST_TAG, "tcp data: %s, len %d from connection %s:%d",
                 data, len, conn->ip, conn->port);
            if (strstr(data, "abc")) {
                parent->handle->send(conn, (const char *)"response of ack from client receiver2", strlen("response of abc from client receiver2") + 1);
                return 0;
            }
            return -1;
        }
    } *receiver2;
    
    
public:
    ClientTest() {
        client = new TcpClient("test_tcp_client", "127.0.0.1", 8888);
        handle = client->getHandle();
        receiver1 = new TcpReceiver1(this);
        handle->addReceiver(receiver1);
        receiver2 = new TcpReceiver2(this);
        handle->addReceiver(receiver2);
    }

    void test()
    {
        if (0 != client->connect()) {
            LOGT("ClientTest", "tcp connect server failed!!!!");
            return;
        }
        TcpConnMgr *conn_mgr = client->getConnMgr();
        STcpConn *conn = conn_mgr->get("127.0.0.1");
        LOGT("ClientTest", "conn is %p, socket is %p", conn, conn->socket);
        while (1) {
            sleep(2);
            int ret = handle->send(conn, "123", strlen("123") + 1);
            LOGT("ClientTest", "send ret %d", ret);
            sleep(2);
            handle->send(conn, "abc", strlen("abc") + 1);
            LOGT("ClientTest", "send ret %d", ret);
        }
    }
};

static ClientTest test;
void TcpClientTest() {
    test.test();
}

//static TcpReceiver1 *receiver = new TcpReceiver1();

