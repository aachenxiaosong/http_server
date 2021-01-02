#include "LiftCtrlService.hpp"
#include "HttpServer.hpp"
#include "UniSerialization.hpp"
#include "UniLog.hpp"

#define LIFT_CTRL_SERVICE_TAG "lift_ctrl_service"

LiftCtrlService :: LiftCtrlService() :
    mWlongHttpHandler("wlong_lift_ctrl_http_handler"),
    mRiliHttpHandler("rili_lift_ctrl_http_handler"),
    mSlingshHttpHandler("slingsh_lift_ctrl_http_handler"),
    mSlingHttpHandler("sling_lift_ctrl_http_handler"),
    mWlongMqHandler("wlong_lift_ctrl_mq_handler")
{
    mWlongHttpHandler.addMessageHandler(&mWlongBookLiftHandler);
    mWlongHttpHandler.addMessageHandler(&mWlongCallLiftHandler);
    mWlongHttpHandler.addMessageHandler(&mWlongBookLiftInterHandler);
    mWlongHttpHandler.addMessageHandler(&mWlongTakeLiftHandler);
    mWlongHttpHandler.addMessageHandler(&mWlongLiftStatusHandler);
    mWlongHttpHandler.addMessageHandler(&mQueryAccessibleFloorsHandler);

    mRiliHttpHandler.addMessageHandler(&mRiliBookLiftHandler);
    mRiliHttpHandler.addMessageHandler(&mRiliCallLiftHandler);
    mRiliHttpHandler.addMessageHandler(&mRiliBookLiftInterHandler);
    mRiliHttpHandler.addMessageHandler(&mRiliTakeLiftHandler);
    mRiliHttpHandler.addMessageHandler(&mRiliLiftStatusHandler);
    mRiliHttpHandler.addMessageHandler(&mQueryAccessibleFloorsHandler);

    mSlingshHttpHandler.addMessageHandler(&mSlingshBookLiftHandler);
    mSlingshHttpHandler.addMessageHandler(&mSlingshCallLiftHandler);
    mSlingshHttpHandler.addMessageHandler(&mSlingshBookLiftInterHandler);
    mSlingshHttpHandler.addMessageHandler(&mQueryAccessibleFloorsHandler);

    mSlingHttpHandler.addMessageHandler(&mSlingBookLiftHandler);
    mSlingHttpHandler.addMessageHandler(&mSlingCallLiftHandler);
    mSlingHttpHandler.addMessageHandler(&mSlingBookLiftInterHandler);
    mSlingHttpHandler.addMessageHandler(&mSlingTakeLiftHandler);
    mSlingHttpHandler.addMessageHandler(&mSlingBookLiftSimpleHandler);
    mSlingHttpHandler.addMessageHandler(&mSlingTakeLiftSimpleHandler);
    mSlingHttpHandler.addMessageHandler(&mQueryAccessibleFloorsHandler);

    mWlongMqHandler.addMessageHandler(&mWlongWechatLiftCtrlHandler);
    mWlongMqHandler.addMessageHandler(&mWlongWechatLiftStatusHandler);

    mVenderType = LIFT_VENDER_NONE;

    vector<MqTopicType> topic_types;
    topic_types.push_back(MQ_TOPIC_LIFT_CTRL_BRAND_CHANGE);
    topic_types.push_back(MQ_TOPIC_LIFT_CTRL_WECHAT);
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

LiftCtrlRequestHandler* LiftCtrlService :: getHttpHandler(LiftVenderType vender_type)
{
    LiftCtrlRequestHandler* handler = NULL;
    switch (vender_type) {
        case LIFT_VENDER_WLONG:
            handler = &mWlongHttpHandler;
            break;
        case LIFT_VENDER_RILI:
            handler = &mRiliHttpHandler;
            break;
        case LIFT_VENDER_SLINGSH:
            handler = &mSlingshHttpHandler;
            break;
        case LIFT_VENDER_SLING:
            handler = &mSlingHttpHandler;
            break;
        default:
            break;
    }
    return handler;
}

LiftCtrlMqHandler* LiftCtrlService :: getMqHandler(LiftVenderType vender_type)
{
    LiftCtrlMqHandler* handler = NULL;
    switch (vender_type) {
        case LIFT_VENDER_WLONG:
            handler = &mWlongMqHandler;
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
    handler = getHttpHandler(mVenderType);
    if (handler != NULL) {
        http_server_del_handler(handler);
    }
    handler = getHttpHandler(vender_type);
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
            string content((const char *)data.content());
            LiftCtrlMessageBrandChange msg = unisound::UniSerialization<LiftCtrlMessageBrandChange>::deseri(content);
            if (msg.brand() == msg.BRAND_WLONG) {
                me->chooseLiftVender(me->LIFT_VENDER_WLONG);
            } else if (msg.brand() == msg.BRAND_RILI) {
                me->chooseLiftVender(me->LIFT_VENDER_RILI);
            } else if (msg.brand() == msg.BRAND_SLINGSH) {
                me->chooseLiftVender(me->LIFT_VENDER_SLINGSH);
            } else if (msg.brand() == msg.BRAND_SLING) {
                me->chooseLiftVender(me->LIFT_VENDER_SLING);
            } else {
                me->chooseLiftVender(me->LIFT_VENDER_NONE);
            }
            LOGT(LIFT_CTRL_SERVICE_TAG, "msg of topic %d is handled OK", MQ_TOPIC_LIFT_CTRL_BRAND_CHANGE);
        } else if (data.topic_type() == MQ_TOPIC_LIFT_CTRL_WECHAT) {
            LiftCtrlMqHandler *handler = me->getMqHandler(me->mVenderType);
            if (handler != NULL) {
                string request((const char *)data.content());
                if (0 == handler->handle(data.topic_type(), request))
                {
                    LOGT(LIFT_CTRL_SERVICE_TAG, "msg of topic %d is handled OK", data.topic_type());
                }
                else
                {
                    LOGT(LIFT_CTRL_SERVICE_TAG, "msg of topic %d is handled failed", data.topic_type());
                }
            }
        }
    }
}
