#include "SulinkRecvLiftCtrlHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkConfigData.hpp"
#include "LiftCtrlMessage.hpp"
#include "UniSerialization.hpp"
#include "UniDeviceInfo.hpp"
#include "UniUtil.hpp"
#include "configurable_info.h"
#include "UniLog.hpp"

#define SULINK_RECV_LIFT_CTRL_TAG getName().c_str()
SulinkRecvLiftCtrlHandler :: SulinkRecvLiftCtrlHandler() : ISulinkMessageHandler("sulink_recv_Lift_ctrl_handler"), mMq("sulink_recv_lift_ctrl_handler_mq") {}

SulinkRecvLiftCtrlHandler :: ~SulinkRecvLiftCtrlHandler() {}

Message * SulinkRecvLiftCtrlHandler :: handle(const Message &message) {
    if (message.type() != MSG_SULINK_RECV_LIFT_CTRL) {
        return NULL;
    }
    const SulinkMessageRecvLiftCtrl& msg = dynamic_cast<const SulinkMessageRecvLiftCtrl&>(message);
    LiftCtrlMessageWechatCtrl *req = new LiftCtrlMessageWechatCtrl();
    req->retcode(msg.errCode());
    req->msg(msg.errMessage());
    req->reqId(msg.reqId());
    req->homeId(msg.homeId());
    req->fromFloor(msg.fromFloor());
    req->toFloor(msg.toFloor());

    
    //string content = unisound::UniSerialization<LiftCtrlMessageWechatCtrl>::seri(req);
    //MqData data(MQ_TOPIC_LIFT_CTRL_WECHAT, (void *)content.c_str(), content.length() + 1);
    //mMq.send(data);
    return 0;
}
