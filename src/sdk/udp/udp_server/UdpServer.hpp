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
public:
    UdpServer(const char* name, int port)
    {
        mName = name;
        mPort = port;
        mThread = NULL;
    }
    ~UdpServer()
    {
        mHandlers.clear();
        if (mThread != NULL)
        {
            mThread->join();
            delete mThread;
        }
    }
    void addHandler(IUdpDataHandler *handler);
    void delHandler(IUdpDataHandler *handler);
    int start();
    bool isStarted() {
        return (mThread != NULL);
    }
    static void recvTask(void *arg);
};