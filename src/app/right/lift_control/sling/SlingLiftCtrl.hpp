#pragma once
#include <string>
#include <vector>

using namespace std;

struct SlingFloor {
    enum OpenDoor {
        FRONT = 1,
        REAR = 2
    } openDoor;
    int floor;
    SlingFloor() {
        openDoor = FRONT;
        this->floor = 0;
    }
    SlingFloor(int floor) {
        openDoor = FRONT;
        this->floor = floor;
    }
    SlingFloor(OpenDoor open_door, int floor) {
        this->openDoor = open_door;
        this->floor = floor;
    }
};
struct SlingRequestAttribute {
    int clusterId;
    //刷卡位置
    enum {
        LOBBY = 1,    //层站
        ENTRANCE = 2, //玄关
        ROOM = 3,     //卧室
        GATE = 4      //闸机
    } position;
    //刷卡类别
    enum {
        OUTOF_CAR = 1,
        IN_CAR = 2
    } type;
    
    enum {
        NORMAL = 0,
        HANDICAPPED = 1,
        VIP = 2,
        MGMT = 3
    } attribute;
    enum {
        AUTO_HALL = 0,
        UNLOCK_HALL_UNLOCK_CAR = 1,
        REGISTER_HALL = 2,
        REGISTER_HALL_UNLOCK_CAR = 3,
        REGISTER_HALL_REGISTER_CAR = 4
    } hallCallMode;
    enum {
        AUTO_CAR = 0,
        UNLOCK_CAR = 1,
        REGISTER_CAR = 2
    } carCallMode;
};

struct SlingResponseAttribute {
    enum {
        REGISTED,
        UNLOCKED,
        FAILED
    } status;
    int elevatorNum;
};

class SlingLiftCtrl {
private:
    string mIp;
    int mPort;
private:
    int udpSend(unsigned char *request, int request_len, unsigned char *response, int *response_len);
    
public:
    SlingLiftCtrl(const string& url);
    int bookElevator(const SlingFloor& from_floor, const SlingFloor& to_floor, const SlingRequestAttribute& request, SlingResponseAttribute& response);
    int bookElevator(const SlingFloor& from_floor, const vector<SlingFloor>& to_floors, const SlingRequestAttribute& request, SlingResponseAttribute& response);
};