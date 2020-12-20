#pragma once

#include "Message.hpp"
#include "MessageType.hpp"
#include <string>
#include <vector>

using namespace std;

#define LIFT_CTRL_UP    "up"
#define LIFT_CTRL_DOWN  "down"

class LiftCtrlMessage : public Message
{
    //对req来讲用来存储unpack处理过程中生成的错误没和错误信息
    MEMBER(int, retcode)
    MEMBER(string, msg)
public:
    LiftCtrlMessage(MessageType type) : Message(type)
    {
        retcode(0);
        msg("OK");
    }
    virtual ~LiftCtrlMessage() {}
};

class LiftCtrlMessageReq : public LiftCtrlMessage {
public:
    LiftCtrlMessageReq(MessageType type) : LiftCtrlMessage(type)
    {}
    ~LiftCtrlMessageReq()
    {}
};
class LiftCtrlMessageRsp : public LiftCtrlMessage {
public:
    LiftCtrlMessageRsp() : LiftCtrlMessage(MSG_LIFT_CTRL_RSP)
    {}
    LiftCtrlMessageRsp(MessageType type) : LiftCtrlMessage(type)
    {}
    ~LiftCtrlMessageRsp()
    {}
};

class LiftCtrlMessageCallLiftReq : public LiftCtrlMessageReq
{
    MEMBER(string, homeId)
    MEMBER(string, upDown)
    MEMBER(int, unlockTime)
public:
    LiftCtrlMessageCallLiftReq() : LiftCtrlMessageReq(MSG_LIFT_CTRL_CALL_LIFT_REQ)
    {
        unlockTime(0);
        upDown(LIFT_CTRL_DOWN);
    }
    ~LiftCtrlMessageCallLiftReq()
    {}
};
class LiftCtrlMessageCallLiftRsp : public LiftCtrlMessageRsp
{
    MEMBER(int, ackCode)
    MEMBER(int, elevatorId)
public:
    LiftCtrlMessageCallLiftRsp() : LiftCtrlMessageRsp(MSG_LIFT_CTRL_CALL_LIFT_RSP)
    {}
    ~LiftCtrlMessageCallLiftRsp()
    {}
};

class LiftCtrlMessageBookLiftReq : public LiftCtrlMessageReq
{
    MEMBER(string, defaultHomeId)
    PMEMBER(vector<string>, authorizedHomeIds)
    MEMBER(string, mode)
    MEMBER(string, deviceCode)
    MEMBER(int, unlockTime)
public:
    const string MODE_OPEN = "open";
    const string MODE_UNLOCK = "unlock";
    const string MODE_ALL = "all";
    const string MODE_NONE = "none";
    LiftCtrlMessageBookLiftReq() : LiftCtrlMessageReq(MSG_LIFT_CTRL_BOOK_LIFT_REQ)
    {
        unlockTime(0);
    }
    ~LiftCtrlMessageBookLiftReq()
    {}
};
class LiftCtrlMessageBookLiftRsp : public LiftCtrlMessageRsp
{
    MEMBER(int, ackCode)
    MEMBER(int, elevatorId)
    public:
    LiftCtrlMessageBookLiftRsp() : LiftCtrlMessageRsp(MSG_LIFT_CTRL_BOOK_LIFT_RSP)
    {}
    ~LiftCtrlMessageBookLiftRsp()
    {}
};

class LiftCtrlMessageBookLiftInterReq : public LiftCtrlMessageReq
{
    MEMBER(string, sHomeId)
    MEMBER(string, dHomeId)
    MEMBER(int, unlockTime)
public:
    LiftCtrlMessageBookLiftInterReq() : LiftCtrlMessageReq(MSG_LIFT_CTRL_BOOK_LIFT_INTER_REQ)
    {
        unlockTime(0);
    }
    ~LiftCtrlMessageBookLiftInterReq()
    {}
};
class LiftCtrlMessageBookLiftInterRsp : public LiftCtrlMessageRsp
{
    MEMBER(int, ackCode)
    MEMBER(int, elevatorId)
public:
    LiftCtrlMessageBookLiftInterRsp() : LiftCtrlMessageRsp(MSG_LIFT_CTRL_BOOK_LIFT_INTER_RSP)
    {}
    ~LiftCtrlMessageBookLiftInterRsp()
    {}
};

class LiftCtrlMessageTakeLiftReq : public LiftCtrlMessageReq
{
    MEMBER(string, defaultHomeId)
    PMEMBER(vector<string>, authorizedHomeIds)
    MEMBER(string, deviceCode)
public:
    LiftCtrlMessageTakeLiftReq() : LiftCtrlMessageReq(MSG_LIFT_CTRL_TAKE_LIFT_REQ)
    {}
    ~LiftCtrlMessageTakeLiftReq()
    {}
};
class LiftCtrlMessageTakeLiftRsp : public LiftCtrlMessageRsp
{
    MEMBER(int, ackCode)
public:
    LiftCtrlMessageTakeLiftRsp() : LiftCtrlMessageRsp(MSG_LIFT_CTRL_TAKE_LIFT_RSP)
    {}
    ~LiftCtrlMessageTakeLiftRsp()
    {}
};

class LiftCtrlMessageLiftStatusReq : public LiftCtrlMessageReq
{
    MEMBER(string, homeId)
    MEMBER(int, elevatorId)
public:
    LiftCtrlMessageLiftStatusReq() : LiftCtrlMessageReq(MSG_LIFT_CTRL_LIFT_STATUS_REQ)
    {}
    ~LiftCtrlMessageLiftStatusReq()
    {}
};
class LiftCtrlMessageLiftStatusRsp : public LiftCtrlMessageRsp
{
    MEMBER(string, curFloor)
    MEMBER(string, direction)
    MEMBER(string, movingStatus)
    MEMBER(string, doorStatus)
public:
    LiftCtrlMessageLiftStatusRsp() : LiftCtrlMessageRsp(MSG_LIFT_CTRL_LIFT_STATUS_RSP)
    {}
    ~LiftCtrlMessageLiftStatusRsp()
    {}
};
//from mq
class LiftCtrlMessageBrandChange : public LiftCtrlMessageReq
{
    MEMBER(string, brand)
SERI_BEGIN
    SERI(brand)
SERI_END
public:
    const string BRAND_WLONG = "wlong";
    const string BRAND_RILI = "rili";
    const string BRAND_SLING = "sling";
    const string BRAND_INVALID = "";
public:
    LiftCtrlMessageBrandChange() : LiftCtrlMessageReq(MSG_LIFT_CTRL_BRAND_CHANGE)
    {
        brand(BRAND_INVALID);
    }
    ~LiftCtrlMessageBrandChange() {}
};

class LiftCtrlMessageWechatCtrl : public LiftCtrlMessageReq
{
    MEMBER(string, reqId)
    MEMBER(string, homeId)
    MEMBER(string, fromFloor)
    MEMBER(string, toFloor)
SERI_BEGIN
    SERI(type)
    SERI(reqId)
    SERI(homeId)
    SERI(fromFloor)
    SERI(toFloor)
SERI_END
public:
    LiftCtrlMessageWechatCtrl() : LiftCtrlMessageReq(MSG_LIFT_CTRL_WECHAT_CTRL)
    {
    }
    ~LiftCtrlMessageWechatCtrl() {}
};

class LiftCtrlMessageWechatCtrlAck : public LiftCtrlMessageRsp
{
    MEMBER(string, reqId)
    MEMBER(string, upDown)
    MEMBER(string, elevatorId)
    MEMBER(string, curFloor)
SERI_BEGIN
    SERI(type)
    SERI(retcode)
    SERI(msg)
    SERI(reqId)
    SERI(upDown)
    SERI(elevatorId)
    SERI(curFloor)
SERI_END
public:
    LiftCtrlMessageWechatCtrlAck() : LiftCtrlMessageRsp(MSG_LIFT_CTRL_WECHAT_CTRL_ACK)
    {
    }
    ~LiftCtrlMessageWechatCtrlAck() {}
};

class LiftCtrlMessageWechatStatus : public LiftCtrlMessageReq
{
    MEMBER(string, reqId)
    MEMBER(string, homeId)
    MEMBER(string, elevatorId)
SERI_BEGIN
    SERI(type)
    SERI(reqId)
    SERI(homeId)
    SERI(elevatorId)
SERI_END
public:
    LiftCtrlMessageWechatStatus() : LiftCtrlMessageReq(MSG_LIFT_CTRL_WECHAT_STATUS)
    {
    }
    ~LiftCtrlMessageWechatStatus() {}
};

class LiftCtrlMessageWechatStatusAck : public LiftCtrlMessageRsp
{
    MEMBER(string, reqId)
    MEMBER(string, curFloor)
    MEMBER(string, direction)
    MEMBER(string, movingStatus)
    MEMBER(string, doorStatus)
SERI_BEGIN
    SERI(type)
    SERI(retcode)
    SERI(msg)
    SERI(reqId)
    SERI(curFloor)
    SERI(direction)
    SERI(movingStatus)
    SERI(doorStatus)
SERI_END
public:
    LiftCtrlMessageWechatStatusAck() : LiftCtrlMessageRsp(MSG_LIFT_CTRL_WECHAT_STATUS_ACK)
    {
    }
    ~LiftCtrlMessageWechatStatusAck() {}
};



