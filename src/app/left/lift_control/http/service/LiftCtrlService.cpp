#include "LiftCtrlService.hpp"
#include "HttpServer.hpp"
#include "UniLog.hpp"

#define LIFT_CTRL_SERVICE_TAG "lift_ctrl_service"

LiftCtrlService :: LiftCtrlService() :
    mWlongHandler("wlong_lift_ctrl_http_handler"),
    mRiliHandler("rili_lift_ctrl_http_handler")
{
    mWlongHandler.addMessageHandler(&mWlongBookLiftHandler);
    mWlongHandler.addMessageHandler(&mWlongCallLiftHandler);
    mWlongHandler.addMessageHandler(&mWlongBookLiftInterHandler);
    mWlongHandler.addMessageHandler(&mWlongTakeLiftHandler);
    mWlongHandler.addMessageHandler(&mWlongLiftStatusHandler);

    mRiliHandler.addMessageHandler(&mRiliBookLiftHandler);
    mRiliHandler.addMessageHandler(&mRiliCallLiftHandler);
    mRiliHandler.addMessageHandler(&mRiliBookLiftInterHandler);
    mRiliHandler.addMessageHandler(&mRiliTakeLiftHandler);
    mRiliHandler.addMessageHandler(&mRiliLiftStatusHandler);
    mVenderType = LIFT_VENDER_NONE;

    vector<MqTopicType> topic_types;
    topic_types.push_back(MQ_TOPIC_LIFT_CTRL_BRAND_CHANGE);
    mMq = new Mq("lift_ctrl_service_mq", topic_types);
    mMqThreadRunning = false;
    mMqThread = new thread(mqRecvTask, this);
}

LiftCtrlService :: ~LiftCtrlService()
{
    mMqThreadRunning = false;
    //TODO: task won't exit immediately
    mMqThread->join();
    delete mMqThread;
    delete mMq;
}

LiftCtrlRequestHandler* LiftCtrlService :: getHandler(LiftVenderType vender_type)
{
    LiftCtrlRequestHandler* handler = NULL;
    switch (vender_type) {
        case LIFT_VENDER_WLONG:
            handler = &mWlongHandler;
            break;
        case LIFT_VENDER_RILI:
            handler = &mRiliHandler;
            break;
        default:
            break;
    }
    return handler;
}


int LiftCtrlService :: chooseLiftVender(LiftVenderType vender_type)
{
    if (mVenderType == vender_type) {
        LOGT(LIFT_CTRL_SERVICE_TAG, "vender type is repeatedly set to %d", vender_type);
        return 0;
    }
    int ret = -1;
    IHttpRequestHandler *handler = NULL;
    handler = getHandler(mVenderType);
    if (handler != NULL) {
        http_server_del_handler(handler);
    }
    handler = getHandler(vender_type);
    if (handler != NULL) {
        LOGT(LIFT_CTRL_SERVICE_TAG, "vender type is changed from %d to %d", mVenderType, vender_type);
        http_server_add_handler(handler);
        mVenderType = vender_type;
        ret = 0;
    } else {
        mVenderType = LIFT_VENDER_NONE;
    }
    return ret;
}

void LiftCtrlService :: mqRecvTask(void *arg) {
    LiftCtrlService *me = (LiftCtrlService *)arg;
    me->mMqThreadRunning = true;
    while (me->mMqThreadRunning) {
        MqData data = me->mMq->recv();
        if (data.topic_type() == MQ_TOPIC_LIFT_CTRL_BRAND_CHANGE)
        {
            LiftCtrlMessageBrandChange* msg = (LiftCtrlMessageBrandChange*)data.content();
            if (msg->brand() == msg->BRAND_WLONG) {
                me->chooseLiftVender(me->LIFT_VENDER_WLONG);
            } else if (msg->brand() == msg->BRAND_RILI) {
                me->chooseLiftVender(me->LIFT_VENDER_RILI);
            } else {
                me->chooseLiftVender(me->LIFT_VENDER_NONE);
            }
            LOGT(LIFT_CTRL_SERVICE_TAG, "MQ_TOPIC_LIFT_CTRL_BRAND_CHANGE is handled");
        }
    }
}
