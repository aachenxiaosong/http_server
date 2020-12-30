#pragma once
#include "UdpServer.hpp"
#include "UdpClient.hpp"
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
        this->floor = -1;
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
    uint8_t seqNum;
    uint8_t clusterId;
    //刷卡位置
    enum {
        LOBBY = 1,    //层站
        ENTRANCE = 2, //玄关
        ROOM = 3,     //卧室
        GATE = 4      //闸机
    } verificationLocation;
    //刷卡类别
    enum {
        OUTOF_CAR = 1,
        IN_CAR = 2
    } verificationType;
    
    enum {
        NORMAL = 0,
        HANDICAPPED = 1,
        VIP = 2,
        MGMT = 3
    } callAttribute;
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

struct SlingResponse {
    int seqNum;
    enum {
        REGISTED,
        UNLOCKED,
        FAILED
    } status;
    int elevatorNum;
};

struct SlingHb {
    int seqNum;
    bool having_data;
    uint8_t data1;
    uint8_t data2;
};

class SlingLiftCtrl : public IUdpDataHandler {
private:
    string mElsgwIp;
    int mElsgwPort;
    static UdpServer mUdpServer;
    static UdpClient mUdpClient;
private:
    //for recved data
    struct {
        enum {
            IDLE,
            EMPTY,
            FULL
        } status;
        SlingResponse rsp;
    } mWaitRsp;
    struct {
        enum {
            IDLE,
            EMPTY,
            FULL
        } status;
        SlingHb hb;
    } mWaitHb;
public:
    SlingLiftCtrl(const string& url);
    ~SlingLiftCtrl();
    int bookElevator(const SlingFloor& from_floor, const SlingFloor& to_floor, const SlingRequestAttribute& request, SlingResponse& response);
    int bookElevator(const SlingFloor& from_floor, const vector<SlingFloor>& to_floors, const SlingRequestAttribute& request, SlingResponse& response);
    int handle(const char*data, int data_len);
};