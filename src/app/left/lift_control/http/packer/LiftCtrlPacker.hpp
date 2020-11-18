#ifndef LEFT_LIFT_CONTROL_HTTP_PACKER_LIFT_CTRL_PACKER_HPP_
#define LEFT_LIFT_CONTROL_HTTP_PACKER_LIFT_CTRL_PACKER_HPP_

#include "LiftCtrlMessage.hpp"

class LiftCtrlPacker {
private:
    LiftCtrlMessageCallLiftReq* unpackCallLiftReq(const string& raw_data);
    LiftCtrlMessageBookLiftReq* unpackBookLiftReq(const string& raw_data);
    LiftCtrlMessageBookLiftInterReq* unpackBookLiftInterReq(const string& raw_data);
    LiftCtrlMessageTakeLiftReq* unpackTakeLiftReq(const string& raw_data);
    LiftCtrlMessageLiftStatusReq* unpackLiftStatusReq(const string& raw_data);
    string* packRsp(const LiftCtrlMessageRsp &message);
    string* packCallLiftRsp(const LiftCtrlMessageCallLiftRsp &message);
    string* packBookLiftRsp(const LiftCtrlMessageBookLiftRsp &message);
    string* packBookLiftInterRsp(const LiftCtrlMessageBookLiftInterRsp &message);
    string* packTakeLiftRsp(const LiftCtrlMessageTakeLiftRsp &message);
    string* packLiftStatusRsp(const LiftCtrlMessageLiftStatusRsp &message);

public:
    LiftCtrlPacker() = default;
    ~LiftCtrlPacker() = default;
    //return message need to be released by app
    LiftCtrlMessageReq* unpack(const string& path, const string& raw_data);
    //return string need to be released by app
    string* pack(const LiftCtrlMessageRsp &message);
};

#endif // LEFT_LIFT_CONTROL_HTTP_PACKER_LIFT_CTRL_PACKER_HPP_
