#pragma once
#include "IUdpDataHandler.hpp"
#include "UniRwLock.hpp"
#include <string>
#include <vector>
#include <thread>

class UdpServer {
private:
    std::string mName;
    int mPort;
    std::vector<IUdpDataHandler*> mHandlers;
    UniRwLock mHandlerLock;
    std::thread *mThread;
    void *mDgs;
public:
    UdpServer(const char* name, int port);
    ~UdpServer();
    void addHandler(IUdpDataHandler *handler);
    void delHandler(IUdpDataHandler *handler);
    int start();
    bool isStarted() {
        return (mThread != NULL);
    }
    int send(const std::string& server_ip, int server_port, const char* data, int data_len);    
    static void recvTask(void *arg);
};