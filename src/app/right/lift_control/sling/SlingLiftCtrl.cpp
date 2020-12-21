#include "SlingLiftCtrl.hpp"
#include "Poco/URI.h"
#include "UniLog.hpp"
#include "uni_iot.h"

#define SLINGSH_LIFT_CTRL_TAG "slingsh_lift_ctrl"

SlingLiftCtrl :: SlingLiftCtrl(const string& url)
{
    Poco::URI uri(url);
    mIp = uri.getHost();
    mPort = uri.getPort();
}


int SlingLiftCtrl :: bookElevator(const SlingFloor& from_floor, const SlingFloor& to_floor, const SlingRequestAttribute& request, SlingResponseAttribute& response)
{
    return 0;

}

int SlingLiftCtrl :: bookElevator(const SlingFloor& from_floor, const vector<SlingFloor>& to_floors, const SlingRequestAttribute& request, SlingResponseAttribute& response)
{

    return 0;
}


int SlingLiftCtrl :: udpSend(unsigned char *request, int request_len, unsigned char *response, int *response_len) {
  return 0;
}
