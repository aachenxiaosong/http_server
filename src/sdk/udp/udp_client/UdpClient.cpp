#include "UdpClient.hpp"
#include "UniLog.hpp"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"

#define UDP_CLIENT_TAG mName.c_str()

int UdpClient :: send(const char* data, int data_len)
{
    int ret = 0;
    try
    {
        Poco::Net::SocketAddress sa(mServerIp, mServerPort);
        Poco::Net::DatagramSocket dgs(Poco::Net::IPAddress::IPv4);
        if (mClientPort > 0) {
            Poco::Net::SocketAddress ca(mServerPort);
            dgs.bind(ca);
        }
        dgs.connect(sa);
        if (data_len == dgs.sendBytes(data, data_len))
        {
            LOGT(UDP_CLIENT_TAG, "udp data sent OK");
        }
        else
        {
            LOGE(UDP_CLIENT_TAG, "udp data sent failed");
            ret = -1;
        }
        dgs.close();
    }
    catch (Poco::Net::HostNotFoundException e)
    {
        LOGE(UDP_CLIENT_TAG, "udp data sent failed for %s", e.what());
        ret = -1;
    }
    catch (Poco::Net::NetException e)
    {
        LOGE(UDP_CLIENT_TAG, "udp data sent failed for %s", e.what());
        ret = -1;
    } catch (...)
    {
        LOGE(UDP_CLIENT_TAG, "udp data sent failed for unknown exception");
        ret = -1;
    }
    return ret;
}

