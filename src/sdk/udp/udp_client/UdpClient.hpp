#pragma once
#include <string>

class UdpClient {
private:
    std::string mName;
    std::string mServerIp;
    int mServerPort;
public:
    UdpClient(const char* name, const char* server_ip, int server_port)
    {
        mName = name;
        mServerIp = server_ip;
        mServerPort = server_port;
    }
    ~UdpClient() = default;
    int send(const char* data, int data_len);
};