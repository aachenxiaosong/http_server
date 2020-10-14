#include "TcpClientTest.hpp"
#include "TcpClient.hpp"
#include "uni_log.h"
#include <string.h>
#include <unistd.h>

#define TCP_CLIENT_TEST_TAG getName().c_str()

class ClientTest
{
private:
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
        int onRecv(const char *data, int len)
        {
            LOGT(TCP_CLIENT_TEST_TAG, "tcp data: %s, len %d from connection %s:%d",
                 data, len, mConn->getIp().c_str(), mConn->getPort());
            if (strstr(data, "123")) {
                mConn->send((const char *)"response of ack from client receiver1", strlen("response of 123 from client receiver1") + 1);
                return 0;
            }
            return -1;
        }
        ITcpReceiver *copy() {
            ITcpReceiver *receiver = new TcpReceiver1(parent);
            receiver->setConn(mConn);
            return receiver;
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
        int onRecv(const char *data, int len)
        {
            LOGT(TCP_CLIENT_TEST_TAG, "tcp data: %s, len %d from connection %s:%d",
                 data, len, mConn->getIp().c_str(), mConn->getPort());
            if (strstr(data, "abc")) {
                mConn->send((const char *)"response of ack from client receiver2", strlen("response of abc from client receiver2") + 1);
                return 0;
            }
            return -1;
        }
        ITcpReceiver *copy() {
            ITcpReceiver *receiver = new TcpReceiver2(parent);
            receiver->setConn(mConn);
            return receiver;
        }
    } *receiver2;
    
    
public:
    ClientTest() {
        client = new TcpClient("test_tcp_client", "127.0.0.1", 8888);
        receiver1 = new TcpReceiver1(this);
        client->addReceiver(receiver1);
        receiver2 = new TcpReceiver2(this);
        client->addReceiver(receiver2);
    }

    void test()
    {
        if (0 != client->connect()) {
            LOGT("tcp_client_test", "tcp connect server failed!!!!");
            return;
        }
        TcpConnMgr *conn_mgr = client->getConnMgr();
        TcpConn *conn = conn_mgr->get("127.0.0.1");
        while (1) {
            int ret;
            sleep(2);
            ret = conn->send("123", strlen("123") + 1);
            LOGT("tcp_client_test", "tcp send result %d", ret);
            sleep(2);
            conn->send("abc", strlen("abc") + 1);
            LOGT("tcp_client_test", "tcp send result %d", ret);
        }
    }
};

static ClientTest test;
void TcpClientTest() {
    test.test();
}

//static TcpReceiver1 *receiver = new TcpReceiver1();

