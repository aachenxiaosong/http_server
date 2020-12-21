#include "SlingLiftCtrl.hpp"
#include "Poco/URI.h"
#include "UniLog.hpp"
#include "uni_iot.h"

#define SLING_LIFT_CTRL_TAG "sling_lift_ctrl"

UdpServer SlingLiftCtrl :: mUdpServer("sling_lift_ctrl_udp_server", 52000);

SlingLiftCtrl :: SlingLiftCtrl(const string& url) : IUdpDataHandler("sling_lift_ctrl_udp_data_handler")
{
    Poco::URI uri(url);
    mElsgwIp = uri.getHost();
    mElsgwPort = uri.getPort();
    mUdpServer.addHandler(this);
    if (mUdpServer.isStarted() == false) {
        mUdpServer.start();
    }
}

SlingLiftCtrl :: ~SlingLiftCtrl()
{
    mUdpServer.delHandler(this);
}

int SlingLiftCtrl :: bookElevator(const SlingFloor& from_floor, const SlingFloor& to_floor, const SlingRequestAttribute& request, SlingResponseAttribute& response)
{
    return 0;
}

int SlingLiftCtrl :: bookElevator(const SlingFloor& from_floor, const vector<SlingFloor>& to_floors, const SlingRequestAttribute& request, SlingResponseAttribute& response)
{

    return 0;
}

int SlingLiftCtrl :: handle(const char*data, int data_len)
{
    return 0;
}

