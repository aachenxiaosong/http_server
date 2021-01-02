#include "LiftCtrlRequestPacker.hpp"
#include "SulinkLiftInitData.hpp"
#include "CJsonObject.hpp"
#include "UniLog.hpp"

#define LIFT_CTRL_PACKER_TAG "lift_ctrl_packer"

using namespace neb;

LiftCtrlMessageCallLiftReq *LiftCtrlRequestPacker ::unpackCallLiftReq(const string &raw_data)
{
    LiftCtrlMessageCallLiftReq *request = new LiftCtrlMessageCallLiftReq();
    CJsonObject jrequest(raw_data);
    string svalue;
    int ivalue;
    if (true != jrequest.Get("homeId", svalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param homeId");
        return request;
    }
    request->homeId(svalue);
    if (true == jrequest.Get("upDown", svalue))
    {
        if ((svalue.compare("up") != 0 && svalue.compare("down") != 0))
        {
            request->retcode(RETCODE_ERROR);
            request->msg("wrong param upDown");
            return request;
        }
        request->upDown(svalue);
    }
    if (true == jrequest.Get("unlockTime", ivalue))
    {
        request->unlockTime(ivalue);
    }
    else
    {
        request->unlockTime(SulinkLiftInitData::getUnlockTime());
    }
    return request;
}

LiftCtrlMessageBookLiftReq *LiftCtrlRequestPacker ::unpackBookLiftReq(const string &raw_data)
{
    LiftCtrlMessageBookLiftReq *request = new LiftCtrlMessageBookLiftReq();
    CJsonObject jrequest(raw_data);
    CJsonObject jvalue;
    string svalue;
    int ivalue;
    /* if (true != jrequest.Get("defaultHomeId", svalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param defaultHomeId");
        return request;
    }
    request->defaultHomeId(svalue); */
    if (true == jrequest.Get("defaultHomeId", svalue)) {
        request->defaultHomeId(svalue);
    }
    if (true == jrequest.Get("authorizedHomeIds", jvalue))
    {
        if (true != jvalue.IsArray())
        {
            request->retcode(RETCODE_ERROR);
            request->msg("wrong param authorizedHomeIds, should be array");
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
         svalue.compare(request->MODE_ALL) != 0 && svalue.compare(request->MODE_NONE) != 0))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param mode");
        return request;
    }
    request->mode(svalue);

    if (true == jrequest.Get("deviceCode", svalue))
    {
        request->deviceCode(svalue);
    }
    if (true == jrequest.Get("unlockTime", ivalue))
    {
        request->unlockTime(ivalue);
    }
    else
    {
        request->unlockTime(SulinkLiftInitData::getUnlockTime());
    }
    return request;
}

LiftCtrlMessageBookLiftInterReq *LiftCtrlRequestPacker ::unpackBookLiftInterReq(const string &raw_data)
{
    LiftCtrlMessageBookLiftInterReq *request = new LiftCtrlMessageBookLiftInterReq();
    CJsonObject jrequest(raw_data);
    string svalue;
    int ivalue;
    if (true != jrequest.Get("sHomeId", svalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param sHomeId");
        return request;
    }
    request->sHomeId(svalue);
    if (true != jrequest.Get("dHomeId", svalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param dHomeId");
        return request;
    }
    request->dHomeId(svalue);
    if (true == jrequest.Get("unlockTime", ivalue))
    {
        request->unlockTime(ivalue);
    }
    else
    {
        request->unlockTime(SulinkLiftInitData::getUnlockTime());
    }
    return request;
}

LiftCtrlMessageTakeLiftReq *LiftCtrlRequestPacker ::unpackTakeLiftReq(const string &raw_data)
{
    LiftCtrlMessageTakeLiftReq *request = new LiftCtrlMessageTakeLiftReq();
    CJsonObject jrequest(raw_data);
    CJsonObject jvalue;
    string svalue;
    int ivalue;
   /*  if (true != jrequest.Get("defaultHomeId", svalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param defaultHomeId");
        return request;
    }
    request->defaultHomeId(svalue); */
    if (true == jrequest.Get("defaultHomeId", svalue))
    {
        request->defaultHomeId(svalue); 
    }
    if (true != jrequest.Get("deviceCode", svalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param deviceCode");
        return request;
    }
    request->deviceCode(svalue);
    if (true == jrequest.Get("authorizedHomeIds", jvalue))
    {
        if (true != jvalue.IsArray())
        {
            request->retcode(RETCODE_ERROR);
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
    return request;
}

LiftCtrlMessageBookLiftSimpleReq* LiftCtrlRequestPacker :: unpackBookLiftSimpleReq(const string& raw_data)
{
    LiftCtrlMessageBookLiftSimpleReq *request = new LiftCtrlMessageBookLiftSimpleReq();
    CJsonObject jrequest(raw_data);
    string svalue;
    if (true != jrequest.Get("toFloor", svalue)) {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param toFloor");
        return request;
    }
    request->toFloor(svalue);
    if (true != jrequest.Get("deviceCode", svalue)) {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param deviceCode");
        return request;
    }
    request->deviceCode(svalue);
    return request;
}

LiftCtrlMessageTakeLiftSimpleReq* LiftCtrlRequestPacker :: unpackTakeLiftSimpleReq(const string& raw_data)
{
    LiftCtrlMessageTakeLiftSimpleReq *request = new LiftCtrlMessageTakeLiftSimpleReq();
    CJsonObject jrequest(raw_data);
    string svalue;
    if (true != jrequest.Get("toFloor", svalue)) {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param toFloor");
        return request;
    }
    request->toFloor(svalue);
    if (true != jrequest.Get("deviceCode", svalue)) {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param deviceCode");
        return request;
    }
    request->deviceCode(svalue);
    return request;
}

LiftCtrlMessageLiftStatusReq* LiftCtrlRequestPacker :: unpackLiftStatusReq(const string &raw_data)
{
    LiftCtrlMessageLiftStatusReq *request = new LiftCtrlMessageLiftStatusReq();
    CJsonObject jrequest(raw_data);
    int ivalue;
    string svalue;
    if (true != jrequest.Get("homeId", svalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param homeId");
        return request;
    }
    request->homeId(svalue);
    if (true != jrequest.Get("elevatorId", ivalue))
    {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param elevatorId");
        return request;
    }
    request->elevatorId(ivalue);
    return request;
}

LiftCtrlMessageQueryAccessibleFloorsReq* LiftCtrlRequestPacker :: unpackQueryAccessibleFloorsReq(const string& raw_data)
{
    LiftCtrlMessageQueryAccessibleFloorsReq *request = new LiftCtrlMessageQueryAccessibleFloorsReq();
    CJsonObject jrequest(raw_data);
    CJsonObject jvalue;
    string svalue;
     if (true != jrequest.Get("deviceCode", svalue)) {
        request->retcode(RETCODE_ERROR);
        request->msg("wrong param deviceCode");
        return request;
    }
    request->deviceCode(svalue);
    if (true == jrequest.Get("authorizedHomeIds", jvalue))
    {
        if (true != jvalue.IsArray())
        {
            request->retcode(RETCODE_ERROR);
            request->msg("wrong param authorizedHomeIds, should be array");
            return request;
        }
        for (int i = 0; i < jvalue.GetArraySize(); i++)
        {
            string home_id;
            jvalue.Get(i, home_id);
            request->authorizedHomeIds().push_back(home_id);
        }
    }
    return request;
}

string *LiftCtrlRequestPacker ::packRsp(const LiftCtrlMessageRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    return new string(jresponse.ToString());
}

string *LiftCtrlRequestPacker ::packCallLiftRsp(const LiftCtrlMessageCallLiftRsp &message)
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

string *LiftCtrlRequestPacker ::packBookLiftRsp(const LiftCtrlMessageBookLiftRsp &message)
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

string *LiftCtrlRequestPacker ::packBookLiftInterRsp(const LiftCtrlMessageBookLiftInterRsp &message)
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

string *LiftCtrlRequestPacker ::packTakeLiftRsp(const LiftCtrlMessageTakeLiftRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.Add("ackCode", message.ackCode());
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

string* LiftCtrlRequestPacker :: packBookLiftSimpleRsp(const LiftCtrlMessageBookLiftSimpleRsp &message)
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

string* LiftCtrlRequestPacker :: packTakeLiftSimpleRsp(const LiftCtrlMessageTakeLiftSimpleRsp &message)
{
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.Add("ackCode", message.ackCode());
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

string* LiftCtrlRequestPacker :: packLiftStatusRsp(const LiftCtrlMessageLiftStatusRsp &message)
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

string* LiftCtrlRequestPacker :: packQueryAccessibleFloorsRsp(const LiftCtrlMessageQueryAccessibleFloorsRsp& message)
{
    LiftCtrlMessageQueryAccessibleFloorsRsp msg = message;
    CJsonObject jresponse;
    jresponse.Add("retcode", message.retcode());
    jresponse.Add("msg", message.msg());
    CJsonObject jdata;
    jdata.AddEmptySubArray("accessibleFloors");
    for (auto floor :  msg.accessibleFloors()) {
        jdata["accessibleFloors"].Add(floor);
    }
    jresponse.Add("data", jdata);
    return new string(jresponse.ToString());
}

LiftCtrlMessageReq *LiftCtrlRequestPacker ::unpack(const string &path, const string &raw_data)
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
    else if (path.compare("/liftCtrl/v3/bookLiftSimple") == 0)
    {
        return unpackBookLiftSimpleReq(raw_data);
    }
    else if (path.compare("/liftCtrl/v3/takeLiftSimple") == 0)
    {
        return unpackTakeLiftSimpleReq(raw_data);
    }
    else if (path.compare("/liftCtrl/v3/liftStatus") == 0)
    {
        return unpackLiftStatusReq(raw_data);
    }
    else if (path.compare("/liftCtrl/v3/queryAccessibleFloors") == 0)
    {
        return unpackQueryAccessibleFloorsReq(raw_data);
    }
    LOGT(LIFT_CTRL_PACKER_TAG, "unsupport request path %s", path.c_str());
    return NULL;
}

string *LiftCtrlRequestPacker ::pack(const LiftCtrlMessageRsp &message)
{
    switch (message.type())
    {
    case MSG_LIFT_CTRL_RSP:
    {
        return packRsp(dynamic_cast<const LiftCtrlMessageRsp &>(message));
    }
    case MSG_LIFT_CTRL_CALL_LIFT_RSP:
    {
        return packCallLiftRsp(dynamic_cast<const LiftCtrlMessageCallLiftRsp &>(message));
    }
    case MSG_LIFT_CTRL_BOOK_LIFT_RSP:
    {
        return packBookLiftRsp(dynamic_cast<const LiftCtrlMessageBookLiftRsp &>(message));
    }
    case MSG_LIFT_CTRL_BOOK_LIFT_INTER_RSP:
    {
        return packBookLiftInterRsp(dynamic_cast<const LiftCtrlMessageBookLiftInterRsp &>(message));
    }
    case MSG_LIFT_CTRL_TAKE_LIFT_RSP:
    {
        return packTakeLiftRsp(dynamic_cast<const LiftCtrlMessageTakeLiftRsp &>(message));
    }
    case MSG_LIFT_CTRL_BOOK_LIFT_SIMPLE_RSP:
    {
        return packBookLiftSimpleRsp(dynamic_cast<const LiftCtrlMessageBookLiftSimpleRsp &>(message));
    }
    case MSG_LIFT_CTRL_TAKE_LIFT_SIMPLE_RSP:
    {
        return packTakeLiftSimpleRsp(dynamic_cast<const LiftCtrlMessageTakeLiftSimpleRsp &>(message));
    }
    case MSG_LIFT_CTRL_LIFT_STATUS_RSP:
    {
        return packLiftStatusRsp(dynamic_cast<const LiftCtrlMessageLiftStatusRsp &>(message));
    }
    case MSG_LIFT_CTRL_QUERY_ACCESSIBLE_FLOORS_RSP:
    {
        return packQueryAccessibleFloorsRsp(dynamic_cast<const LiftCtrlMessageQueryAccessibleFloorsRsp &>(message));
    }
    }
    LOGT(LIFT_CTRL_PACKER_TAG, "unsupport response type %d", message.type());
    return NULL;
}
