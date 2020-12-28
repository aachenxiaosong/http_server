#pragma once
#include <string>

class UdpClient {
private:
    std::string mName;
    std::string mServerIp;
    int mServerPort;
    int mClientPort;
public:
    UdpClient(const char* name, const char* server_ip, int server_port, int client_port = -1)
    {
        mName = name;
        mServerIp = server_ip;
        mServerPort = server_port;
        mClientPort = client_port;
    }
    ~UdpClient() = default;
    int send(const char* data, int data_len);
};