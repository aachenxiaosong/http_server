#pragma once
#include <string>

class UdpClient {
private:
    std::string mName;
    int mClientPort;
    void *mDgs;
public:
    UdpClient(const char* name, int client_port = -1);
    ~UdpClient();
    int send(const std::string& server_ip, int server_port, const char* data, int data_len);
};
