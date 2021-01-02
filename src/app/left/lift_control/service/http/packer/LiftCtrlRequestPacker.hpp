#pragma once

#include "LiftCtrlMessage.hpp"

class LiftCtrlRequestPacker {
private:
    LiftCtrlMessageCallLiftReq* unpackCallLiftReq(const string& raw_data);
    LiftCtrlMessageBookLiftReq* unpackBookLiftReq(const string& raw_data);
    LiftCtrlMessageBookLiftInterReq* unpackBookLiftInterReq(const string& raw_data);
    LiftCtrlMessageTakeLiftReq* unpackTakeLiftReq(const string& raw_data);
    LiftCtrlMessageBookLiftSimpleReq* unpackBookLiftSimpleReq(const string& raw_data);
    LiftCtrlMessageTakeLiftSimpleReq* unpackTakeLiftSimpleReq(const string& raw_data);
    LiftCtrlMessageLiftStatusReq* unpackLiftStatusReq(const string& raw_data);
    string* packRsp(const LiftCtrlMessageRsp &message);
    string* packCallLiftRsp(const LiftCtrlMessageCallLiftRsp &message);
    string* packBookLiftRsp(const LiftCtrlMessageBookLiftRsp &message);
    string* packBookLiftInterRsp(const LiftCtrlMessageBookLiftInterRsp &message);
    string* packTakeLiftRsp(const LiftCtrlMessageTakeLiftRsp &message);
    string* packBookLiftSimpleRsp(const LiftCtrlMessageBookLiftSimpleRsp &message);
    string* packTakeLiftSimpleRsp(const LiftCtrlMessageTakeLiftSimpleRsp &message);
    string* packLiftStatusRsp(const LiftCtrlMessageLiftStatusRsp &message);

public:
    LiftCtrlRequestPacker() = default;
    ~LiftCtrlRequestPacker() = default;
    //return message need to be released by app
    LiftCtrlMessageReq* unpack(const string& path, const string& raw_data);
    //return string need to be released by app
    string* pack(const LiftCtrlMessageRsp &message);
};
