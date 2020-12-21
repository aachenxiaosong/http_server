#include "UdpClient.hpp"
#include "UniLog.hpp"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"

#define UDP_CLIENT_TAG mName.c_str()

int UdpClient :: send(const char* data, int data_len)
{
    int ret = 0;
    Poco::Net::SocketAddress sa(mServerIp, mServerPort);
    Poco::Net::DatagramSocket dgs(Poco::Net::IPAddress::IPv4);
    dgs.connect(sa);
    if (data_len == dgs.sendBytes(data, data_len)) {
        LOGT(UDP_CLIENT_TAG, "udp data sent OK");
    } else {
        LOGE(UDP_CLIENT_TAG, "udp data sent failed");
        ret = -1;
    }
    dgs.close();
    return ret;
}

