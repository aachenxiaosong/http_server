#include "TcpServerTest.hpp"
#include "TcpClientTest.hpp"
#include "DechangServer.hpp"

#define TEST_NUM 1
void AppTest() {
#if (TEST_NUM == 0)
    TcpServerTest();
    TcpClientTest();
#elif (TEST_NUM == 1)
    DechangServer server;
    server.run();
#endif
}