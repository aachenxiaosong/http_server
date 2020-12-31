#include "SulinkPacker.hpp"
#include "SulinkMessage.hpp"
#include "SulinkTraceid.hpp"
#include "UniUtil.hpp"
#include "UniLog.hpp"

#define SULINK_PACKER_TAG getName().c_str()

SulinkPacker :: SulinkPacker() : IMqttPacker("sulink_packer") {}

SulinkPacker :: ~SulinkPacker() {}


string* SulinkPacker :: packSendDeviceInfo(const SulinkMessageSendDeviceInfo& message) {
    CJsonObject jheader;
    //long timestamp = unisound::UniUtil::timestampMsL();
    //jheader.Add("traceId", SulinkTraceid::build(to_string(timestamp)));
    jheader.Add("traceId", message.traceId());
    jheader.Add("payloadVersion", message.payloadVersion());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jdevice;
    jdevice.Add("appKey", message.appKey());
    jdevice.Add("deviceSn", message.deviceSn());
    jdevice.Add("upTime", message.upTime());
    jdevice.Add("appVersion", message.appVersion());
    jdevice.Add("deviceType", message.deviceType());
    CJsonObject jpayload;
    jpayload.AddEmptySubArray("devices");
    jpayload["devices"].Add(jdevice);
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

string* SulinkPacker :: packSendPassRecord(const SulinkMessageSendPassRecord& message) {
    CJsonObject jheader;
    jheader.Add("traceId", message.traceId());
    jheader.Add("payloadVersion", message.payloadVersion());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jrecord;
    jrecord.Add("code", message.code());
    jrecord.Add("extData", message.extData());
    jrecord.Add("passPhoto", message.passPhoto());
    jrecord.Add("passResult", message.passResult());
    jrecord.Add("passTime", message.passTime());
    jrecord.Add("passType", message.passType());
    jrecord.Add("personId", message.personId());
    jrecord.Add("personName", message.personName());
    jrecord.Add("personTemp", message.personTemp());
    jrecord.Add("personType", message.personType());
    CJsonObject jpayload;
    jpayload.AddEmptySubArray("passRecords");
    jpayload["passRecords"].Add(jrecord);
    jpayload.Add("deviceCode", message.deviceCode());
    jpayload.Add("reqId", message.reqId());
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

string* SulinkPacker :: packRecvLiftInfo(const SulinkMessageRecvLiftInfo& message) {
    SulinkMessageRecvLiftInfo& msg = (SulinkMessageRecvLiftInfo&)message;
    CJsonObject jheader;
    jheader.Add("traceId", msg.traceId());
    jheader.Add("payloadVersion", msg.payloadVersion());
    jheader.Add("brand", msg.brand());
    jheader.Add("timestamp", msg.timestamp());
    jheader.Add("method", msg.method());
    CJsonObject jpayload;
    jpayload.Add("reqId", msg.reqId());
    jpayload.Add("deviceCode", msg.deviceCode());
    jpayload.Add("unlockTime", msg.unlockTime());
    jpayload.AddEmptySubArray("spaces");
    for (int i = 0; i < msg.spaces().size(); i++) {
        CJsonObject jspace;
        jspace.Add("id", msg.spaces()[i].id());
        jspace.Add("spaceName", msg.spaces()[i].spaceName());
        jspace.Add("pid", msg.spaces()[i].pid());
        jspace.Add("elevatorSpaceType", msg.spaces()[i].elevatorSpaceType());
        jspace.Add("elevatorSpaceNo", msg.spaces()[i].elevatorSpaceNo());
        jspace.Add("clusterControllerIds", msg.spaces()[i].clusterControllerIds());
        jspace.Add("floorLocation", msg.spaces()[i].floorLocation());
        jspace.Add("timestamp", msg.spaces()[i].timestamp());
        jpayload["spaces"].Add(jspace);
    }
    CJsonObject jbrand_info;
    jbrand_info.Add("brandName", msg.brandInfo().brandName());
    jbrand_info.Add("brandCode", msg.brandInfo().brandCode());
    jbrand_info.Add("intranetUrl", msg.brandInfo().intranetUrl());
    jbrand_info.Add("appId", msg.brandInfo().appId());
    jbrand_info.Add("appSecret", msg.brandInfo().appSecret());
    jbrand_info.Add("licence", msg.brandInfo().licence());
    jbrand_info.Add("schema", msg.brandInfo().schema());
    jbrand_info.AddEmptySubArray("clusterControllers");
    for (int i = 0; i < msg.brandInfo().clusterControllers().size(); i++) {
        CJsonObject jcluster;
        jcluster.Add("controllerIp", msg.brandInfo().clusterControllers()[i].controllerIp());
        jcluster.Add("controllerId", msg.brandInfo().clusterControllers()[i].controllerId());
        jbrand_info["clusterControllers"].Add(jcluster);
    }
    jpayload.Add("brandInfo", jbrand_info);
    jpayload.AddEmptySubArray("childDevices");
    for (int i = 0; i < msg.accessDevices().size(); i++) {
        CJsonObject jdevice;
        jdevice.Add("deviceCode", msg.accessDevices()[i].deviceCode());
        jdevice.Add("deviceSpaceId", msg.accessDevices()[i].spaceId());
        jdevice.Add("workMode", msg.accessDevices()[i].workMode());
        jdevice.Add("floorMap", msg.accessDevices()[i].floorMap());
        jdevice.Add("publicFloor", msg.accessDevices()[i].publicFloor());
        jdevice.Add("curFloor", msg.accessDevices()[i].curFloor());
        jdevice.Add("liftId", msg.accessDevices()[i].liftId());
        jdevice.Add("hallNo", msg.accessDevices()[i].hallNo());
        jdevice.Add("timestamp", msg.accessDevices()[i].timestamp());
        jpayload["childDevices"].Add(jdevice);
    }
    jpayload.Add("timestamp", msg.timestamp());
    jpayload.Add("extData", msg.extData());
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

string* SulinkPacker :: packRecvLiftInfoAck(const SulinkMessageRecvLiftInfoAck& message) {
    CJsonObject jheader;
    jheader.Add("traceId", message.traceId());
    jheader.Add("payloadVersion", message.payloadVersion());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    CJsonObject jpayload;
    jpayload.Add("deviceCode", message.deviceCode());
    jpayload.Add("reqId", message.reqId());
    jpayload.Add("ackReqId", message.ackReqId());
    jpayload.Add("code", message.code());
    jpayload.Add("message", message.message());
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

string* SulinkPacker :: packRecvLiftCtrlAck(const SulinkMessageRecvLiftCtrlAck& message) {
    CJsonObject jheader;
    jheader.Add("traceId", message.traceId());
    jheader.Add("payloadVersion", message.payloadVersion());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    CJsonObject jpayload;
    jpayload.Add("reqId", message.reqId());
    jpayload.Add("ackReqId", message.ackReqId());
    jpayload.Add("deviceCode", message.deviceCode());
    CJsonObject jdata;
    jdata.Add("upDown", message.upDown());
    jdata.Add("elevatorId", message.elevatorId());
    jdata.Add("curFloor", message.curFloor());
    jdata.Add("code", message.code());
    jdata.Add("message", message.message());
    jpayload.Add("data", jdata);
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

string* SulinkPacker :: packRecvLiftStatusAck(const SulinkMessageRecvLiftStatusAck& message) {
    CJsonObject jheader;
    jheader.Add("traceId", message.traceId());
    jheader.Add("payloadVersion", message.payloadVersion());
    jheader.Add("brand", message.brand());
    jheader.Add("timestamp", message.timestamp());
    jheader.Add("method", message.method());
    CJsonObject jmessage;
    jmessage.Add("header", jheader);
    CJsonObject jpayload;
    jpayload.Add("reqId", message.reqId());
    jpayload.Add("ackReqId", message.ackReqId());
    jpayload.Add("deviceCode", message.deviceCode());
    CJsonObject jdata;
    jdata.Add("curFloor", message.curFloor());
    jdata.Add("direction", message.direction());
    jdata.Add("movingStatus", message.movingStatus());
    jdata.Add("doorStatus", message.doorStatus());
    jdata.Add("code", message.code());
    jdata.Add("message", message.message());
    jpayload.Add("data", jdata);
    jmessage.Add("payload", jpayload);
    return new string(jmessage.ToString());
}

int SulinkPacker :: checkPackHeader(const string& raw_data) {
    CJsonObject jmessage(raw_data);
    CJsonObject jheader;
    CJsonObject jvalue;
    long lvalue;
    if (true != jmessage.Get("header", jheader)) {
        LOGE(SULINK_PACKER_TAG, "check header failed");
        return -1;
    }
    if (true != jheader.Get("traceId", jvalue) ||
        true != jheader.Get("payloadVersion", lvalue) ||
        true != jheader.Get("brand", jvalue) ||
        true != jheader.Get("timestamp", lvalue) ||
        true != jheader.Get("method", jvalue)) {
        LOGE(SULINK_PACKER_TAG, "check header content failed");
        return -1;
    }
    return 0;
}

SulinkMessageRecvPassRule* SulinkPacker :: unpackRecvPassRule(const string& raw_data) {
    //TODO
    return NULL;
}

SulinkMessageSendPassRecordAck* SulinkPacker :: unpackSendPassRecordAck(const string& raw_data) {
    CJsonObject jack(raw_data);
    CJsonObject jheader;
    jack.Get("header", jheader);
    string svalue;
    int ivalue;
    long lvalue;
    SulinkMessageSendPassRecordAck* ack = new SulinkMessageSendPassRecordAck();
    jack.Get("traceId", svalue);
    ack->traceId(svalue);
    jack.Get("payloadVersion", ivalue);
    ack->payloadVersion(ivalue);
    jack.Get("timestamp", lvalue);
    ack->timestamp(lvalue);
    CJsonObject jpayload;
    if (true == jack.Get("payload", jpayload)) {
        if (true == jpayload.Get("code", ivalue)) {
            ack->code(ivalue);
        }
    }
    return ack;
}

SulinkMessageRecvLiftInfo* SulinkPacker :: unpackRecvLiftInfo(const string& raw_data) {
    string err_message = "";
    CJsonObject jmessage(raw_data);
    CJsonObject jpayload;
    CJsonObject jvalue;
    string svalue;
    int ivalue;
    long lvalue;
    CJsonObject jspaces;
    CJsonObject Jbrand_info;
    CJsonObject jclusters;
    CJsonObject jaccess_devices;
    SulinkMessageRecvLiftInfo info;
    if (jmessage.Get("payload", jpayload) != true) {
        //LOGE(SULINK_PACKER_TAG, "parse payload failed");
        //return NULL;
        err_message = "parse payload failed";
        goto L_ERROR;
    }
    if (jpayload.Get("reqId", svalue) != true) {
        //LOGE(SULINK_PACKER_TAG, "parse reqId failed");
        //return NULL;
        err_message = "parse reqId failed";
        goto L_ERROR;
    }
    info.reqId(svalue);
    if (jpayload.Get("deviceCode", svalue) != true) {
        //LOGE(SULINK_PACKER_TAG, "parse deviceCode failed");
        //return NULL;
        err_message = "parse deviceCode failed";
        goto L_ERROR;
    }
    info.deviceCode(svalue);
    if (jpayload.Get("unlockTime", ivalue) != true) {
        err_message = "parse unlockTime failed";
        goto L_ERROR;
    }
    info.unlockTime(ivalue);
    if (jpayload.Get("spaces", jspaces) != true) {
        //LOGE(SULINK_PACKER_TAG, "parse spaces failed");
        //return NULL;
        err_message = "parse spaces failed";
        goto L_ERROR;
    }
    for (int i = 0; i < jspaces.GetArraySize(); i++) {
        CJsonObject jspace;
        LiftInfoSpace space;
        jspaces.Get(i, jspace);
        if (jspace.Get("id", svalue) != true) {
            //LOGE(SULINK_PACKER_TAG, "parse space id failed, i=%d", i);
            //return NULL;
            err_message = "parse spaces.id failed, i:" + to_string(i);
            goto L_ERROR;
        }
        space.id(svalue);
        if (jspace.Get("elevatorSpaceType", svalue) != true) {
            //LOGE(SULINK_PACKER_TAG, "parse space elevatorSpaceType failed, i=%d", i);
            //return NULL;
            err_message = "parse spaces.elevatorSpaceType failed, id:" + space.id();
            goto L_ERROR;
        }
        space.elevatorSpaceType(svalue);
        if (jspace.Get("pid", svalue) != true) {
            //LOGE(SULINK_PACKER_TAG, "parse space pid failed, i=%d", i);
            //return NULL;
            err_message = "parse spaces.pid failed, id:" + space.id() + " type:" + space.elevatorSpaceType();
            goto L_ERROR;
        }
        space.pid(svalue);
        if (jspace.Get("spaceName", svalue) != true) {
            //LOGE(SULINK_PACKER_TAG, "parse space spaceName failed, i=%d", i);
            //return NULL;
            err_message = "parse spaces.spaceName failed, id:" + space.id() + " type:" + space.elevatorSpaceType();
            goto L_ERROR;
        }
        space.spaceName(svalue);
        if (jspace.Get("elevatorSpaceNo", svalue) != true) {
            //LOGE(SULINK_PACKER_TAG, "parse space elevatorSpaceNo failed, i=%d", i);
            //return NULL;
            err_message = "parse spaces.elevatorSpaceNo failed, id:" + space.id() + " type:" + space.elevatorSpaceType();
            goto L_ERROR;
        }
        space.elevatorSpaceNo(svalue);
        if (jspace.Get("clusterControllerIds", svalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse spaces.clusterControllerIds failed, id:%s type:%s", space.id().c_str(), space.elevatorSpaceType().c_str());
        } else {
            space.clusterControllerIds(svalue);
        }
        if (jspace.Get("floorLocation", svalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse spaces.floorLocation failed, id:%s type:%s", space.id().c_str(), space.elevatorSpaceType().c_str());
        } else {
            space.floorLocation(svalue);
        }
        if (jspace.Get("timestamp", lvalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse spaces.timestamp failed, id:%s type:%s", space.id().c_str(), space.elevatorSpaceType().c_str());
        } else {
            space.timestamp(lvalue);
        }
        info.spaces().push_back(space);
    }
    //for (vector<LiftInfoSpace> :: iterator it = )
    if (jpayload.Get("brandInfo", Jbrand_info) != true) {
        //LOGE(SULINK_PACKER_TAG, "parse brandInfo failed");
        //return NULL;
        err_message = "parse brandInfo failed";
        goto L_ERROR;
    }
    if (Jbrand_info.Get("brandName", svalue) != true)
    {
        //LOGE(SULINK_PACKER_TAG, "parse brandName failed");
        //return NULL;
        err_message = "parse brandInfo.brandName failed";
        goto L_ERROR;
    }
    info.brandInfo().brandName(svalue);
    if (Jbrand_info.Get("brandCode", svalue) != true)
    {
        //LOGE(SULINK_PACKER_TAG, "parse brandCode failed");
        //return NULL;
        err_message = "parse brandInfo.brandCode failed";
        goto L_ERROR;
    }
    info.brandInfo().brandCode(svalue);
    if (Jbrand_info.Get("intranetUrl", svalue) != true)
    {
        LOGW(SULINK_PACKER_TAG, "parse brandInfo.intranetUrl failed");
    } else {
        info.brandInfo().intranetUrl(svalue);
    }
    if (Jbrand_info.Get("appId", svalue) != true)
    {
        LOGW(SULINK_PACKER_TAG, "parse brandInfo.appId failed");
    } else {
        info.brandInfo().appId(svalue);
    }
    if (Jbrand_info.Get("appSecret", svalue) != true)
    {
        LOGW(SULINK_PACKER_TAG, "parse brandInfo.appSecret failed");
    } else {
        info.brandInfo().appSecret(svalue);
    }
    if (Jbrand_info.Get("licence", svalue) != true)
    {
        LOGW(SULINK_PACKER_TAG, "parse brandInfo.licence failed");
    } else {
        info.brandInfo().licence(svalue);
    }
    if (Jbrand_info.Get("schema", svalue) != true)
    {
        LOGW(SULINK_PACKER_TAG, "parse brandInfo.schema failed");
    } else {
        info.brandInfo().schema(svalue);
    }
    if (Jbrand_info.Get("clusterControllers", jclusters) != true)
    {
        LOGW(SULINK_PACKER_TAG, "parse brandInfo.clusterControllers failed");
    } else {
        CJsonObject jcluster;
        LiftInfoCluster cluster;
        for (int i = 0; i < jclusters.GetArraySize(); i++) {
            jclusters.Get(i, jcluster);
            if (jcluster.Get("controllerId", svalue) != true) {
                //LOGE(SULINK_PACKER_TAG, "parse cluster id failed, i=%d", i);
                //return NULL;
                err_message = "parse clusterControllers.id failed, i:" + to_string(i);
                goto L_ERROR;
            }
            cluster.controllerId(svalue);
            if (jcluster.Get("controllerIp", svalue) != true) {
                //LOGE(SULINK_PACKER_TAG, "parse cluster ip failed, i=%d", i);
                //return NULL;
                err_message = "parse clusterControllers.ip failed, id:" + cluster.controllerId();
                goto L_ERROR;
            }
            cluster.controllerIp(svalue);
            info.brandInfo().clusterControllers().push_back(cluster);
        }
    }
    if (jpayload.Get("childDevices", jaccess_devices) != true) {
        //LOGE(SULINK_PACKER_TAG, "parse childDevices failed");
        //return NULL;
        err_message = "parse childDevices failed";
        goto L_ERROR;
    }
    for (int i = 0; i < jaccess_devices.GetArraySize(); i++) {
        CJsonObject jdevice;
        LiftInfoAccessDevice device;
        jaccess_devices.Get(i, jdevice);
        if (jdevice.Get("deviceCode", svalue) != true) {
            //LOGE(SULINK_PACKER_TAG, "parse device deviceCode failed");
            //return NULL;
            err_message = "parse childDevices.deviceCode failed, i:" + to_string(i);
            goto L_ERROR;
        }
        device.deviceCode(svalue);
        if (jdevice.Get("deviceSpaceId", ivalue) != true) {
            err_message = "parse childDevices.deviceSpaceId failed, i:" + to_string(i);
            goto L_ERROR;
        }
        device.spaceId(ivalue);
        if (jdevice.Get("workMode", ivalue) != true) {
            //LOGE(SULINK_PACKER_TAG, "parse device workMode failed");
            //return NULL;
            err_message = "parse childDevices.workMode failed, deviceCode:" + device.deviceCode();
            goto L_ERROR;
        }
        device.workMode(ivalue);
        if (jdevice.Get("floorMap", svalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse childDevices.floorMap failed, deviceCode:%s", device.deviceCode().c_str());
        } else {
            device.floorMap(svalue);
        }
        if (jdevice.Get("publicFloor", svalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse childDevices.publicFloor failed, deviceCode:%s", device.deviceCode().c_str());
        } else {
            device.publicFloor(svalue);
        }
        if (jdevice.Get("curFloor", svalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse childDevices.curFloor failed, deviceCode:%s", device.deviceCode().c_str());
        } else {
            device.curFloor(svalue);
        }
        if (jdevice.Get("liftId", svalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse childDevices.liftId failed, deviceCode:%s", device.deviceCode().c_str());
        } else {
            device.liftId(svalue);
        }
        if (jdevice.Get("hallNo", svalue) != true) {
            LOGW(SULINK_PACKER_TAG, "parse childDevices.hallNo failed, deviceCode:%s", device.deviceCode().c_str());
        } else {
            device.hallNo(svalue);
        }
        if (jdevice.Get("timestamp", lvalue) != true) {
            err_message = "parse childDevices.timestamp failed, deviceCode:%s" + device.deviceCode();
            goto L_ERROR;
        }
        device.timestamp(lvalue);
        info.accessDevices().push_back(device);
    }
    if (jpayload.Get("timestamp", lvalue) != true) {
        //LOGE(SULINK_PACKER_TAG, "parse timestamp failed");
        //return NULL;
        err_message = "parse info timestamp failed";
        goto L_ERROR;
    }
    info.timestamp(lvalue);
    if (jpayload.Get("extData", svalue) != true) {
        LOGW(SULINK_PACKER_TAG, "parse extData failed");
    } else {
        info.extData(svalue);
    }
    
L_ERROR:
    if (err_message.empty()) {
        info.errCode(0);
    } else {
        LOGE(SULINK_PACKER_TAG, "%s", err_message.c_str());
        info.errCode(RETCODE_ERROR);
        info.errMessage(err_message);
    }
    return new SulinkMessageRecvLiftInfo(info);
}


SulinkMessageRecvLiftCtrl* SulinkPacker :: unpackRecvLiftCtrl(const string& raw_data) {
    string err_message = "";
    CJsonObject jmessage(raw_data);
    CJsonObject jpayload;
    CJsonObject jdata;
    CJsonObject jvalue;
    string svalue;
    long lvalue;
    SulinkMessageRecvLiftCtrl req;
    if (jmessage.Get("payload", jpayload) != true) {
        err_message = "parse payload failed";
        goto L_ERROR;
    }
    if (jpayload.Get("reqId", svalue) != true) {
        err_message = "parse reqId failed";
        goto L_ERROR;
    }
    req.reqId(svalue);
    if (jpayload.Get("deviceCode", svalue) != true) {
        err_message = "parse deviceCode failed";
        goto L_ERROR;
    }
    req.deviceCode(svalue);
    if (jpayload.Get("bookType", svalue) != true) {
        err_message = "parse bookType failed";
        goto L_ERROR;
    }
    req.bookType(svalue);
    if (jpayload.Get("bookData", jdata) != true) {
        err_message = "parse bookData fialed";
        goto L_ERROR;
    }
    if (jdata.Get("homeId", svalue) != true) {
        err_message = "parse bookData.homeId failed";
        goto L_ERROR;
    }
    req.homeId(svalue);
    if (jdata.Get("fromFloor", svalue) != true) {
        err_message = "parse bookData.fromFloor failed";
        goto L_ERROR;
    }
    req.fromFloor(svalue);
    if (jdata.Get("toFloor", svalue) != true) {
        err_message = "parse bookData.toFloor failed";
        goto L_ERROR;
    }
    req.toFloor(svalue);
L_ERROR:
    if (err_message.empty()) {
        req.errCode(0);
    } else {
        LOGE(SULINK_PACKER_TAG, "%s", err_message.c_str());
        req.errCode(RETCODE_ERROR);
        req.errMessage(err_message);
    }
    return new SulinkMessageRecvLiftCtrl(req);
}

SulinkMessageRecvLiftStatus* SulinkPacker :: unpackRecvLiftStatus(const string& raw_data) {
    string err_message = "";
    CJsonObject jmessage(raw_data);
    CJsonObject jpayload;
    CJsonObject jvalue;
    string svalue;
    long lvalue;
    SulinkMessageRecvLiftStatus req;
    if (jmessage.Get("payload", jpayload) != true) {
        err_message = "parse payload failed";
        goto L_ERROR;
    }
    if (jpayload.Get("reqId", svalue) != true) {
        err_message = "parse reqId failed";
        goto L_ERROR;
    }
    req.reqId(svalue);
    if (jpayload.Get("deviceCode", svalue) != true) {
        err_message = "parse deviceCode failed";
        goto L_ERROR;
    }
    req.deviceCode(svalue);
    if (jpayload.Get("homeId", svalue) != true) {
        err_message = "parse homeId failed";
        goto L_ERROR;
    }
    req.homeId(svalue);
    if (jpayload.Get("elevatorId", svalue) != true) {
        err_message = "parse elevatorId failed";
        goto L_ERROR;
    }
    req.elevatorId(svalue);
L_ERROR:
    if (err_message.empty()) {
        req.errCode(0);
    } else {
        LOGE(SULINK_PACKER_TAG, "%s", err_message.c_str());
        req.errCode(RETCODE_ERROR);
        req.errMessage(err_message);
    }
    return new SulinkMessageRecvLiftStatus(req);
}

IMqttMessage* SulinkPacker :: unpack(const string& raw_data) {
    if (checkPackHeader(raw_data) != 0) {
        return NULL;
    }
    string message_type;
    CJsonObject(raw_data)["header"].Get("method", message_type);
    if (message_type.compare("pass-rule-info") == 0) {
        return unpackRecvPassRule(raw_data);
    } else if (message_type.compare("pass-record-ack") == 0) {
        return unpackSendPassRecordAck(raw_data);
    } else if (message_type.compare("device-ubox-lift-info") == 0) {
        return unpackRecvLiftInfo(raw_data);
    } else if (message_type.compare("device-ubox-lift-book") == 0) {
        return unpackRecvLiftCtrl(raw_data);
    } else if (message_type.compare("device-ubox-lift-status") == 0) {
        return unpackRecvLiftStatus(raw_data);
    }
    return NULL;
}

string* SulinkPacker :: pack(const IMqttMessage &message) {
    string *data = NULL;
    switch (message.type()) {
        case MSG_SULINK_SEND_DEVICE_INFO:
            data = packSendDeviceInfo(dynamic_cast<const SulinkMessageSendDeviceInfo&>(message));
            break;
        case MSG_SULINK_SEND_PASS_RECORD:
            data = packSendPassRecord(dynamic_cast<const SulinkMessageSendPassRecord&>(message));
            break;
        case MSG_SULINK_RECV_LIFT_INFO:
            data = packRecvLiftInfo(dynamic_cast<const SulinkMessageRecvLiftInfo&>(message));
            break;
        case MSG_SULINK_RECV_LIFT_INFO_ACK:
            data = packRecvLiftInfoAck(dynamic_cast<const SulinkMessageRecvLiftInfoAck&>(message));
            break;
        case MSG_SULINK_RECV_LIFT_CTRL_ACK:
            data = packRecvLiftCtrlAck(dynamic_cast<const SulinkMessageRecvLiftCtrlAck&>(message));
            break;
        case MSG_SULINK_RECV_LIFT_STATUS_ACK:
            data = packRecvLiftStatusAck(dynamic_cast<const SulinkMessageRecvLiftStatusAck&>(message));
            break;
        default:
            break;
    }
    return data;    
}
