#include "TcpServerTest.hpp"
#include "TcpServer.hpp"
#include "uni_log.h"
#include <string.h>

#define TCP_REQ_HANDLER_TAG getName().c_str()

class TcpDataHandler : public ITcpDataHandler {
public:
    TcpDataHandler() : ITcpDataHandler("test_tcp_requst_handler") {}
    ~TcpDataHandler() {}
    int handle(char *request, int req_len, char *response, int *resp_len) {
        LOGT(TCP_REQ_HANDLER_TAG, "tcp request: %s, len %d", request, req_len);
        sprintf(response, "%s", "response from test handler");
        *resp_len = strlen("response from test handler") + 1;
        return 0;
    }
};

static TcpServer server("test_tcp_server", 8888);
static TcpDataHandler handler;
void TcpServerTest() {
    ITcpDataHandler *h = &handler;
    server.addDataHandler(h);
    server.listen();
}