#include "UdpServer.hpp"
#include "UniLog.hpp"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"

#define UDP_SERVER_TAG mName.c_str()

void UdpServer :: addHandler(IUdpDataHandler *handler)
{
    mHandlerLock.writeLock();
    mHandlers.push_back(handler);
    mHandlerLock.writeUnlock();
}

void UdpServer :: delHandler(IUdpDataHandler *handler)
{
    mHandlerLock.writeLock();
    for (auto it = mHandlers.begin(); it != mHandlers.end(); ++it) {
        if (*it == handler) {
            mHandlers.erase(it);
            break;
        }
    }
    mHandlerLock.writeUnlock();
}

int UdpServer:: start()
{
    if (mThread != NULL) {
        LOGT(UDP_SERVER_TAG, "udp server repeatedly started");
        return 0;
    }
    mThread = new std::thread(recvTask, this);
    LOGT(UDP_SERVER_TAG, "udp server started");
    return 0;
}


#define UDP_SERVER_TAG1 me->mName.c_str()

void UdpServer:: recvTask(void *arg)
{
    UdpServer *me = (UdpServer*)arg;
    try
    {
        Poco::Net::SocketAddress sa(me->mPort);
        Poco::Net::DatagramSocket dgs;
        dgs.bind(sa, true, true);
        char recv_buf[1500];
        int recv_len;
        while (1)
        {
            Poco::Net::SocketAddress sender;
            recv_len = dgs.receiveFrom(recv_buf, sizeof(recv_buf), sender);
            //LOGT(UDP_SERVER_TAG1, "udp data recved, size=%d", recv_len);
            me->mHandlerLock.readLock();
            for (auto handler : me->mHandlers)
            {
                if (0 == handler->handle((const char *)recv_buf, recv_len))
                {
                    LOGT(UDP_SERVER_TAG1, "udp data is handled by %s", handler->getName().c_str());
                    break;
                }
            }
            me->mHandlerLock.readUnlock();
        }
        dgs.close();
    }
    catch (Poco::Net::HostNotFoundException e)
    {
        LOGE(UDP_SERVER_TAG1, "udp server recv failed for %s", e.what());
    }
    catch (Poco::Net::NetException e)
    {
        LOGE(UDP_SERVER_TAG1, "udp server recv failed for %s", e.what());
    } catch (...)
    {
        LOGE(UDP_SERVER_TAG1, "udp server recv failed for unknown exception");
    }
}