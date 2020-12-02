#include "LiftCtrlPacker.hpp"
#include "CJsonObject.hpp"
#include "uni_log.h"

#define LIFT_CTRL_PACKER_TAG "lift_ctrl_packer"

using namespace neb;

LiftCtrlMessageCallLiftReq* LiftCtrlPacker :: unpackCallLiftReq(const string& raw_data)
{
    LiftCtrlMessageCallLiftReq* request = new LiftCtrlMessageCallLiftReq();
    CJsonObject jrequest(raw_data);
    string svalue;
    int ivalue;
    if (true != jrequest.Get("homeId", svalue)) {
        request->retcode(-1);
        request->msg("wrong param homeId");
        return request;
    }
    request->homeId(svalue);
    if (true == jrequest.Get("upDown", svalue)) {
        if ((svalue.compare("up") != 0 && svalue.compare("down") != 0)) {
            request->retcode(-1);
            request->msg("wrong param upDown");
            return request;
        }
        request->upDown(svalue);
    }
    if (true == jrequest.Get("unlockTime", ivalue)) {
        request->unlockTime(ivalue);
    }
    return request;
}

LiftCtrlMessageBookLiftReq* LiftCtrlPacker :: unpackBookLiftReq(const string& raw_data)
{
    LiftCtrlMessageBookLiftReq* request = new LiftCtrlMessageBookLiftReq();
    CJsonObject jrequest(raw_data);
    CJsonObject jvalue;
    string svalue;
    int ivalue;
    if (true != jrequest.Get("defaultHomeId", svalue)) {
        request->retcode(-1);
        request->msg("wrong param defaultHomeId");
        return request;
    }
    request->defaultHomeId(svalue);

    if (true == jrequest.Get("authorizedHomeIds", jvalue))
    {
        if (true != jvalue.IsArray())
        {
            request->retcode(-1);
            request->msg("wrong param authorizedHomeIds");
            return request;
        }
        for (int i = 0; i < jvalue.GetArraySize(); i++)
        {
            string home_id;
            jvalue.Get(i, home_id);
            request->authorizedHomeIds().push_back(home_id);
        }
    }

     if (true != jrequest.Get("mode", svalue) ||
         (svalue.compare(request->MODE_OPEN) != 0 && svalue.compare(request->MODE_UNLOCK) != 0 &&
          svalue.compare(request->MODE_ALL) != 0 && svalue.compare(request->MODE_NONE) != 0) ) {
        request->retcode(-1);
        request->msg("wrong param mode");
        return request;
    }
    request->mode(svalue);
    
    if (true == jrequest.Get("deviceCode", svalue)) {
        request->deviceCode(svalue);
    }
    if (true == jrequest.Get("unlockTime", ivalue)) {
        request->unlockTime(ivalue);
    }
    return request;
}

LiftCtrlMessageBookLiftInterReq* LiftCtrlPacker :: unpackBookLiftInterReq(const string& raw_data)
{
    LiftCtrlMessageBookLiftInterReq* request = new LiftCtrlMessageBookLiftInterReq();
    CJsonObject jrequest(raw_data);
    string svalue;
    int ivalue;
    if (true != jrequest.Get("sHomeId", svalue)) {
        request->retcode(-1);
        request->msg("wrong param sHomeId");
        return request;
    }
    request->sHomeId(svalue);
    if (true != jrequest.Get("dHomeId", svalue)) {
        request->retcode(-1);
        request->msg("wrong param dHomeId");
        return request;
    }
    request->dHomeId(svalue);
    if (true == jrequest.Get("unlockTime", ivalue)) {
        request->unlockTime(ivalue);
    }
    return request;
}

LiftCtrlMessageTakeLiftReq* LiftCtrlPacker :: unpackTakeLiftReq(const string& raw_data)
{
    LiftCtrlMessageTakeLiftReq* request = new LiftCtrlMessageTakeLiftReq();
    CJsonObject jrequest(raw_data);
    CJsonObject jvalue;
    string svalue;
    int ivalue;
    if (true != jrequest.Get("defaultHomeId", svalue)) {
        request->retcode(-1);
        request->msg("wrong param defaultHomeId");
        return request;
    }
    request->defaultHomeId(svalue);
    if (true != jrequest.Get("deviceCode", svalue)) {
        request->retcode(-1);
        request->msg("wrong param deviceCode");
        return request;
    }
    request->deviceCode(svalue);
    if (true == jrequest.Get("authorizedHomeIds", jvalue)) {
        if (true != jvalue.IsArray())
        {
            request->retcode(-1);
            request->msg("wrong param authorizedHomeIds");
            return request;
        }
        for (int i = 0; i < jvalue.GetArraySize(); i++) {
            string home_id;
            jvalue.Get(i, home_id);
            request->authorizedHomeIds().push_back(home_id);
        }
    } 
    return request;
}

LiftCtrlMessageLiftStatusReq* LiftCtrlPacker :: unpackLiftStatusReq(const string& raw_data)
{
    LiftCtrlMessageLiftStatusReq* request = new LiftCtrlMessageLiftStatusReq();
    CJsonObject jrequest(raw_data);
    int ivalue;
    string svalue;
    if (true != jrequest.Get("homeId", svalue)) {
        request->retcode(-1);
        request->msg("wrong param homeId");
        return request;
    }
    request->homeId(svalue);
    if (true != jrequest.Get("elevatorId", ivalue)) {
        request->retcode(-1);
        request->msg("wrong param elevatorId");
        return request;
    }
    request->elevatorId(ivalue);
    return request;
}

string* LiftCtrlPacker :: packRsp(const LiftCtrlMessageRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    return new string(jresponse.ToString());
}

string* LiftCtrlPacker :: packCallLiftRsp(const LiftCtrlMessageCallLiftRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.Add("ackCode", message.ackCode());
    jdata.Add("elevatorId", message.elevatorId());
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

string* LiftCtrlPacker :: packBookLiftRsp(const LiftCtrlMessageBookLiftRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.Add("ackCode", message.ackCode());
    jdata.Add("elevatorId", message.elevatorId());
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

string* LiftCtrlPacker :: packBookLiftInterRsp(const LiftCtrlMessageBookLiftInterRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.Add("ackCode", message.ackCode());
    jdata.Add("elevatorId", message.elevatorId());
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

string* LiftCtrlPacker :: packTakeLiftRsp(const LiftCtrlMessageTakeLiftRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.Add("ackCode", message.ackCode());
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

string* LiftCtrlPacker :: packLiftStatusRsp(const LiftCtrlMessageLiftStatusRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.Add("curFloor", message.curFloor());
    jdata.Add("direction", message.direction());
    jdata.Add("movingStatus", message.movingStatus());
    jdata.Add("doorStatus", message.doorStatus());
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

LiftCtrlMessageReq* LiftCtrlPacker :: unpack(const string& path, const string& raw_data)
{
    if (path.compare("/liftCtrl/v3/callLift") == 0)
    {
        return unpackCallLiftReq(raw_data);
    }
    else if (path.compare("/liftCtrl/v3/bookLift") == 0)
    {
        return unpackBookLiftReq(raw_data);
    }
    else if (path.compare("/liftCtrl/v3/bookLiftInter") == 0)
    {
        return unpackBookLiftInterReq(raw_data);
    }
    else if (path.compare("/liftCtrl/v3/takeLift") == 0)
    {
        return unpackTakeLiftReq(raw_data);
    }
    else if (path.compare("/liftCtrl/v3/liftStatus") == 0)
    {
        return unpackLiftStatusReq(raw_data);
    }
    LOGT(LIFT_CTRL_PACKER_TAG, "unsupport request path %s", path.c_str());
    return NULL;
}

string* LiftCtrlPacker :: pack(const LiftCtrlMessageRsp &message)
{
    switch (message.type()) {
        case MSG_LIFT_CTRL_RSP: {
            return packRsp(dynamic_cast<const LiftCtrlMessageRsp&>(message));
        }
        case MSG_LIFT_CTRL_CALL_LIFT_RSP: {
            return packCallLiftRsp(dynamic_cast<const LiftCtrlMessageCallLiftRsp&>(message));
        }
        case MSG_LIFT_CTRL_BOOK_LIFT_RSP: {
            return packBookLiftRsp(dynamic_cast<const LiftCtrlMessageBookLiftRsp&>(message));
        }
        case MSG_LIFT_CTRL_BOOK_LIFT_INTER_RSP: {
            return packBookLiftInterRsp(dynamic_cast<const LiftCtrlMessageBookLiftInterRsp&>(message));
        }
        case MSG_LIFT_CTRL_TAKE_LIFT_RSP: {
            return packTakeLiftRsp(dynamic_cast<const LiftCtrlMessageTakeLiftRsp&>(message));
        }
        case MSG_LIFT_CTRL_LIFT_STATUS_RSP: {
            return packLiftStatusRsp(dynamic_cast<const LiftCtrlMessageLiftStatusRsp&>(message));
        }
    }
    LOGT(LIFT_CTRL_PACKER_TAG, "unsupport response type %d", message.type());
    return NULL;
}