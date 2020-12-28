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
struct SlingDataHeader {
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
struct SlingReqBodySingleFloor {
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
    BYTE reserve2;
    BYTE reserve3;
};
#pragma pack()

#pragma pack(1)
struct SlingReqBodyMultiFloor {
    BYTE command_number;
    BYTE data_length;
    WORD device_number;
    BYTE verification_type;
    BYTE verification_location;
    BYTE button_attribute;
    BYTE reserve1;
    WORD boarding_floor;
    BYTE reserve2;
    BYTE boarding_front_rear;
    BYTE reserve3;
    BYTE call_attribute;
    BYTE nonstop_operation;
    BYTE call_registration_mode;
    BYTE sequence_number;
    BYTE front_destination_floor_data_length;
    BYTE rear_destination_floor_data_length;
    BYTE destination_floors[64]; //注：按需要填充，最大长度为64
};
#pragma pack()

#pragma pack(1)
struct SlingRspBody {
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
struct SlingReqSingleFloor {
    SlingDataHeader header;
    SlingReqBodySingleFloor body;
};
#pragma pack()

#pragma pack(1)
struct SlingReqMultiFloor {
    SlingDataHeader header;
    SlingReqBodyMultiFloor body;
};
#pragma pack()

UdpServer SlingLiftCtrl :: mUdpServer("sling_lift_ctrl_udp_server", 52000);

SlingLiftCtrl :: SlingLiftCtrl(const string& url) : IUdpDataHandler("sling_lift_ctrl_udp_data_handler")
{
    Poco::URI uri(url);
    mElsgwIp = uri.getHost();
    mElsgwPort = uri.getPort();
    LOGE(SLING_LIFT_CTRL_TAG, "sling lift controller created, ip %s port %d", mElsgwIp.c_str(), mElsgwPort);
    mUdpServer.addHandler(this);
    if (mUdpServer.isStarted() == false) {
        mUdpServer.start();
        LOGT(SLING_LIFT_CTRL_TAG, "sling lift controller udp server started");
    }
}

SlingLiftCtrl :: ~SlingLiftCtrl()
{
    mUdpServer.delHandler(this);
}


int SlingLiftCtrl :: bookElevator(const SlingFloor& from_floor, const SlingFloor& to_floor, const SlingRequestAttribute& request, SlingResponseAttribute& response)
{
    SlingReqSingleFloor req = {0};
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
    UdpClient client("sling_book_elevator_udp_client", mElsgwIp.c_str(), mElsgwPort, 52000);
    if (0 != client.send((const char*)&req, sizeof(req))) {
        LOGE(SLING_LIFT_CTRL_TAG, "send sling book lift request failed");
        return -1;
    }
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

