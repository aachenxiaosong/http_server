#include "UdpClient.hpp"
#include "UniLog.hpp"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"

#define UDP_CLIENT_TAG mName.c_str()

UdpClient :: UdpClient(const char *name, int client_port)
{
    mName = name;
    mClientPort = client_port;
    Poco::Net::DatagramSocket *dgs = new Poco::Net::DatagramSocket(Poco::Net::IPAddress::IPv4);
    //Poco::Net::DatagramSocket dgs(Poco::Net::IPAddress::IPv4);
    if (mClientPort > 0)
    {
        Poco::Net::SocketAddress ca(mClientPort);
        dgs->bind(ca, true, true);
    }
    mDgs = (void *)dgs;
}

UdpClient :: ~UdpClient()
{
    Poco::Net::DatagramSocket *dgs = (Poco::Net::DatagramSocket *)mDgs;
    dgs->close();
    delete dgs;
}


int UdpClient :: send(const string& server_ip, int server_port, const char* data, int data_len)
{
    int ret = 0;
    Poco::Net::DatagramSocket *dgs = (Poco::Net::DatagramSocket *)mDgs;
    try
    {
        Poco::Net::SocketAddress sa(server_ip, server_port);
        //if (data_len == dgs.sendBytes(data, data_len))
        if (data_len == dgs->sendTo(data, data_len, sa))
        {
            LOGT(UDP_CLIENT_TAG, "udp data sent OK");
        }
        else
        {
            LOGE(UDP_CLIENT_TAG, "udp data sent failed");
            ret = -1;
        }
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

