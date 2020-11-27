#ifndef HTTP_SERVICE_WLONG_WLONG_3PARTY_LIFT_CTRL_H_
#define HTTP_SERVICE_WLONG_WLONG_3PARTY_LIFT_CTRL_H_

#include "CJsonObject.hpp"
#include <string>

#define CALL_ELEVATOR_BY_FLOOR_PATH "/eiserver/client/systemDockingDeviceInfo/callElevatorByFoor"
#define BOOKING_ELEVATOR_PATH            "/eiserver/client/systemDockingDeviceInfo/BookingElevator"
#define RESERVE_ELEVATOR_PATH       "/eiserver/client/systemDockingDeviceInfo/reserveCallElevator"
#define CONTROLLER_INFO_PATH             "/eiserver/client/systemDockingDeviceInfo/getCloudControllerInfo"
#define WLONG_UP                                   "2"
#define WLONG_DN                                   "1"
using namespace neb;
using namespace std;

class WlongResponse {
    public:
    int code;
    string msg;
    string data;
};

class WlongLiftCtrl {
    private:
    string url;
    string appid;
    string appsecret;
    string licence;
    int _parse_result(char *result, WlongResponse& response);
    public:
    WlongLiftCtrl(const string& url, const string& appid, const string& appsecret, const string& licence);
    int callElevatorByFoor(int lift_id, string& open_floors, string& unlock_floors, WlongResponse& response);
    int bookingElevator(int cluster_id, string& from_floor, string& updown, string& open_floors, int open_time, WlongResponse& response);
    int reserveElevator(int cluster_id, string& from_floor, string& open_floor, WlongResponse& response);
    int aliveTest(string &message);
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_3PARTY_LIFT_CTRL_H_