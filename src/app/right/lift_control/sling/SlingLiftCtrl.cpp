#include "SlingLiftCtrl.hpp"
#include "Poco/URI.h"
#include "UniLog.hpp"
#include "uni_iot.h"
#include <netinet/in.h>

#define SLING_LIFT_CTRL_TAG "sling_lift_ctrl"

typedef char     CHAR;
typedef uint8_t  BYTE;
typedef int8_t   BCD;
typedef uint16_t WORD;
typedef uint32_t DWORD;

#pragma pack(1)
struct _SlingDataHeader {
    WORD identifier;
    WORD data_length;
    BYTE address_device_type;
    BYTE address_device_number;
    BYTE sender_device_type;
    BYTE sender_device_number;
    BYTE reserve[4];
};
#pragma pack()

#pragma pack(1)
struct _SlingReqBodySingleFloor {
    BYTE command_number;
    BYTE data_length;
    WORD device_number;
    BYTE verification_type;
    BYTE verification_location;
    BYTE button_attribute;
    BYTE reserve1;
    WORD boarding_floor;
    WORD destination_floor;
    BYTE boarding_front_rear;
    BYTE destination_front_rear;
    BYTE call_attribute;
    BYTE nonstop_operation;
    BYTE call_registration_mode;
    BYTE sequence_number;
    BYTE reserve[2];
};
#pragma pack()

#pragma pack(1)
struct _SlingReqBodyMultiFloor {
    BYTE command_number;
    BYTE data_length;
    WORD device_number;
    BYTE verification_type;
    BYTE verification_location;
    BYTE button_attribute;
    BYTE reserve1;
    WORD boarding_floor;
    BYTE reserve2[2];
    BYTE boarding_front_rear;
    BYTE reserve3;
    BYTE call_attribute;
    BYTE nonstop_operation;
    BYTE call_registration_mode;
    BYTE sequence_number;
    BYTE front_destination_floor_data_length;
    BYTE rear_destination_floor_data_length;
    BYTE front_destination_floors[32];
    BYTE rear_destination_floors[32];
};
#pragma pack()

#pragma pack(1)
struct _SlingDataBody {
    BYTE command_number;
    BYTE reserve[7];
};
#pragma pack()

#pragma pack(1)
struct _SlingRspBody {
    BYTE command_number;
    BYTE data_length;
    WORD device_number;
    BYTE acceptance_status;
    BYTE assigned_elevator_car_number;
    BYTE sequence_number;
    BYTE assignment_attribute;
};
#pragma pack()

#pragma pack(1)
struct _SlingHbBody {
    BYTE command_number;
    BYTE data_length;
    BYTE having_data_towareds_elevator_system;
    BYTE data1;
    BYTE data2;
    BYTE reserve[3];
};
#pragma pack()

#pragma pack(1)
struct _SlingReqSingleFloor {
    _SlingDataHeader header;
    _SlingReqBodySingleFloor body;
};
#pragma pack()

#pragma pack(1)
struct _SlingReqMultiFloor {
    _SlingDataHeader header;
    _SlingReqBodyMultiFloor body;
};
#pragma pack()

#pragma pack(1)
struct _SlingData {
    _SlingDataHeader header;
    _SlingDataBody body;
};
#pragma pack()

#pragma pack(1)
struct _SlingRsp {
    _SlingDataHeader header;
    _SlingRspBody body;
};
#pragma pack()

#pragma pack(1)
struct _SlingHb {
    _SlingDataHeader header;
    _SlingHbBody body;
};
#pragma pack()


UdpServer SlingLiftCtrl :: mUdpServer("sling_lift_ctrl_udp_server", 52000);

SlingLiftCtrl :: SlingLiftCtrl(const string& url) : IUdpDataHandler("sling_lift_ctrl_udp_data_handler")
{
    Poco::URI uri(url);
    mElsgwIp = uri.getHost();
    mElsgwPort = uri.getPort();
    LOGT(SLING_LIFT_CTRL_TAG, "sling lift controller created, ip %s port %d", mElsgwIp.c_str(), mElsgwPort);
    mUdpServer.addHandler(this);
    mWaitHb.status = mWaitHb.IDLE;
    mWaitRsp.status = mWaitRsp.IDLE;
    if (mUdpServer.isStarted() == false) {
        mUdpServer.start();
        LOGT(SLING_LIFT_CTRL_TAG, "sling lift controller udp server started");
    }
}

SlingLiftCtrl :: ~SlingLiftCtrl()
{
    mUdpServer.delHandler(this);
}


int SlingLiftCtrl :: bookElevator(const SlingFloor& from_floor, const SlingFloor& to_floor, const SlingRequestAttribute& request, SlingResponse& response)
{
    _SlingReqSingleFloor req = {0};
    req.header.identifier = htons(0x1730);
    req.header.data_length = htons(20);//整个body的长度
    req.header.address_device_type = 0x01;//ELSGW
    req.header.address_device_number = request.clusterId;
    req.header.sender_device_type = 0x11; // ACS
    req.header.sender_device_number = 1; //写死
    req.body.command_number = 0x01;
    req.body.data_length = 18;
    req.body.device_number = htons(0);
    req.body.verification_type = request.verificationType;
    req.body.verification_location = request.verificationLocation;
    req.body.button_attribute = 16; // auto
    req.body.boarding_floor = htons(from_floor.floor);
    req.body.destination_floor = htons(to_floor.floor);
    req.body.boarding_front_rear = from_floor.openDoor;
    req.body.destination_front_rear = to_floor.openDoor;
    req.body.call_attribute = request.callAttribute;
    req.body.nonstop_operation = 0;
    req.body.call_registration_mode = request.hallCallMode;
    req.body.sequence_number = request.seqNum;
    if (0 != mUdpServer.send(mElsgwIp, mElsgwPort, (const char*)&req, sizeof(req))) {
        LOGE(SLING_LIFT_CTRL_TAG, "send sling book lift request failed");
        return -1;
    }
    int wait_time = 3000;
    mWaitRsp.status = mWaitRsp.EMPTY;
    while (wait_time > 0) {
        usleep(50 * 1000);
        wait_time -= 50;
        if (mWaitRsp.status == mWaitRsp.FULL) {
            if (mWaitRsp.rsp.seqNum == request.seqNum) {
                break;
            } else {
                //continue to wait
                LOGT(SLING_LIFT_CTRL_TAG, "recv sling book lift response, but seq does not match(%d %d), continue to wait", mWaitRsp.rsp.seqNum, request.seqNum);
                mWaitRsp.status = mWaitRsp.IDLE;
            }
        }
    }
    if (mWaitRsp.status == mWaitRsp.FULL) {
        mWaitRsp.status = mWaitRsp.IDLE;
        response = mWaitRsp.rsp;
        LOGT(SLING_LIFT_CTRL_TAG, "recv sling book lift response OK, elevator number is %d", response.elevatorNum);
        return 0;
    } else {
        LOGE(SLING_LIFT_CTRL_TAG, "recv sling book lift response timeout");
    }
    return -1;
}

int SlingLiftCtrl :: bookElevator(const SlingFloor& from_floor, const vector<SlingFloor>& to_floors, const SlingRequestAttribute& request, SlingResponse& response)
{
    _SlingReqMultiFloor req = {0};
    req.header.identifier = htons(0x1730);
    req.header.data_length = htons(84);//整个body的长度
    req.header.address_device_type = 0x01;//ELSGW
    req.header.address_device_number = request.clusterId;
    req.header.sender_device_type = 0x11; // ACS
    req.header.sender_device_number = 1; //写死
    req.body.command_number = 0x02;
    req.body.data_length = 82;
    req.body.device_number = htons(0);
    req.body.verification_type = request.verificationType;
    req.body.verification_location = request.verificationLocation;
    req.body.button_attribute = 16; // auto
    req.body.boarding_floor = htons(from_floor.floor);
    req.body.boarding_front_rear = from_floor.openDoor;
    req.body.call_attribute = request.callAttribute;
    req.body.nonstop_operation = 0;
    req.body.call_registration_mode = request.hallCallMode;
    req.body.sequence_number = request.seqNum;
    
    req.body.front_destination_floor_data_length = 32;
    req.body.rear_destination_floor_data_length = 32;
    for (auto floor : to_floors) {
        if (floor.openDoor == floor.FRONT) {
            req.body.front_destination_floors[(floor.floor - 1) / 8] |= (1 << ((floor.floor - 1) % 8));
        } else {
            req.body.rear_destination_floors[(floor.floor - 1) / 8] |= (1 << ((floor.floor - 1) % 8));
        }
    }
    if (0 != mUdpServer.send(mElsgwIp, mElsgwPort, (const char*)&req, sizeof(req))) {
        LOGE(SLING_LIFT_CTRL_TAG, "send sling book lift request failed");
        return -1;
    }
    int wait_time = 3000;
    mWaitRsp.status = mWaitRsp.EMPTY;
    while (wait_time > 0) {
        usleep(50 * 1000);
        wait_time -= 50;
        if (mWaitRsp.status == mWaitRsp.FULL) {
            if (mWaitRsp.rsp.seqNum == request.seqNum) {
                break;
            } else {
                //continue to wait
                LOGT(SLING_LIFT_CTRL_TAG, "recv sling book lift response, but seq does not match(%d %d), continue to wait", mWaitRsp.rsp.seqNum, request.seqNum);
                mWaitRsp.status = mWaitRsp.IDLE;
            }
        }
    }
    if (mWaitRsp.status == mWaitRsp.FULL) {
        mWaitRsp.status = mWaitRsp.IDLE;
        response = mWaitRsp.rsp;
        LOGT(SLING_LIFT_CTRL_TAG, "recv sling book lift response OK, elevator number is %d", response.elevatorNum);
        return 0;
    } else {
        LOGE(SLING_LIFT_CTRL_TAG, "recv sling book lift response timeout");
    }
    return -1;
}

int SlingLiftCtrl :: handle(const char*data, int data_len)
{
    _SlingData *sling_data = (_SlingData *)data;
    if (ntohs(sling_data->header.identifier) != 0x1730) {
        LOGW(SLING_LIFT_CTRL_TAG, "invalid data");
        return -1;
    }
    if (sling_data->body.command_number == 0x81 && mWaitRsp.status == mWaitRsp.EMPTY) {
        _SlingRsp *sling_rsp = (_SlingRsp *)data;
        mWaitRsp.rsp.seqNum = sling_rsp->body.sequence_number;
        mWaitRsp.rsp.elevatorNum = sling_rsp->body.assigned_elevator_car_number;
        if (sling_rsp->body.acceptance_status == 0x00) {
            mWaitRsp.rsp.status = mWaitRsp.rsp.REGISTED;
        } else if (sling_rsp->body.acceptance_status == 0x01) {
            mWaitRsp.rsp.status = mWaitRsp.rsp.UNLOCKED;
        } else {
            mWaitRsp.rsp.status = mWaitRsp.rsp.FAILED;
        }
        mWaitRsp.status = mWaitRsp.FULL;
        return 0;
    }
    if (sling_data->body.command_number == 0xf1 && mWaitHb.status == mWaitHb.EMPTY) {
        _SlingHb *sling_hb = (_SlingHb *)data;
        mWaitHb.hb.having_data = sling_hb->body.having_data_towareds_elevator_system;
        mWaitHb.hb.data1 = sling_hb->body.data1;
        mWaitHb.hb.data2 = sling_hb->body.data2;
        mWaitHb.status = mWaitHb.FULL;
        return 0;
    }
    return -1;
}

