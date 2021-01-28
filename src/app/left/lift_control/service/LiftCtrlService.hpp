#pragma once

#include "LiftCtrlRequestHandler.hpp"
#include "QueryAccessibleFloorsMessageHandler.hpp"
#include "WlongBookLiftMessageHandler.hpp"
#include "WlongCallLiftMessageHandler.hpp"
#include "WlongBookLiftInterMessageHandler.hpp"
#include "WlongTakeLiftMessageHandler.hpp"
#include "WlongLiftStatusMessageHandler.hpp"
#include "WlongBookLiftSimpleMessageHandler.hpp"
#include "WlongTakeLiftSimpleMessageHandler.hpp"
#include "RiliBookLiftMessageHandler.hpp"
#include "RiliCallLiftMessageHandler.hpp"
#include "RiliBookLiftInterMessageHandler.hpp"
#include "RiliTakeLiftMessageHandler.hpp"
#include "RiliLiftStatusMessageHandler.hpp"
#include "SlingshBookLiftMessageHandler.hpp"
#include "SlingshCallLiftMessageHandler.hpp"
#include "SlingshBookLiftInterMessageHandler.hpp"
#include "SlingBookLiftMessageHandler.hpp"
#include "SlingCallLiftMessageHandler.hpp"
#include "SlingBookLiftInterMessageHandler.hpp"
#include "SlingTakeLiftMessageHandler.hpp"
#include "SlingBookLiftSimpleMessageHandler.hpp"
#include "SlingTakeLiftSimpleMessageHandler.hpp"
#include "LiftCtrlMqHandler.hpp"
#include "WlongWechatCtrlMessageHandler.hpp"
#include "WlongWechatStatusMessageHandler.hpp"
#include "Mq.hpp"
#include <thread>


class LiftCtrlService {
private:
    Mq *mMq;
    thread *mMqThread;
    bool mMqThreadRunning;
    //common http message handlers
    QueryAccessibleFloorsMessageHandler mQueryAccessibleFloorsHandler;
   //wanglong http message handlers
    WlongBookLiftMessageHandler mWlongBookLiftHandler;
    WlongCallLiftMessageHandler mWlongCallLiftHandler;
    WlongBookLiftInterMessageHandler mWlongBookLiftInterHandler;
    WlongTakeLiftMessageHandler mWlongTakeLiftHandler;
    WlongLiftStatusMessageHandler mWlongLiftStatusHandler;
	WlongBookLiftSimpleMessageHandler mWlongBookLiftSimpleHandler;
	WlongTakeLiftSimpleMessageHandler mWlongTakeLiftSimpleHandler;
    LiftCtrlRequestHandler mWlongHttpHandler;
    //rili http message handlers
    RiliBookLiftMessageHandler mRiliBookLiftHandler;
    RiliCallLiftMessageHandler mRiliCallLiftHandler;
    RiliBookLiftInterMessageHandler mRiliBookLiftInterHandler;
    RiliTakeLiftMessageHandler mRiliTakeLiftHandler;
    RiliLiftStatusMessageHandler mRiliLiftStatusHandler;
    LiftCtrlRequestHandler mRiliHttpHandler;
    //sanling shanghai http message handlers
    SlingshBookLiftMessageHandler mSlingshBookLiftHandler;
    SlingshCallLiftMessageHandler mSlingshCallLiftHandler;
    SlingshBookLiftInterMessageHandler mSlingshBookLiftInterHandler;
    LiftCtrlRequestHandler mSlingshHttpHandler;
    //sanling http message handlers
    SlingBookLiftMessageHandler mSlingBookLiftHandler;
    SlingCallLiftMessageHandler mSlingCallLiftHandler;
    SlingBookLiftInterMessageHandler mSlingBookLiftInterHandler;
    SlingTakeLiftMessageHandler mSlingTakeLiftHandler;
    SlingBookLiftSimpleMessageHandler mSlingBookLiftSimpleHandler;
    SlingTakeLiftSimpleMessageHandler mSlingTakeLiftSimpleHandler;
    LiftCtrlRequestHandler mSlingHttpHandler;
    //wanglong wechat msg handler
    WlongWechatCtrlMessageHandler mWlongWechatLiftCtrlHandler;
    WlongWechatStatusMessageHandler mWlongWechatLiftStatusHandler;
    LiftCtrlMqHandler mWlongMqHandler;

public:
    enum LiftVenderType {
        LIFT_VENDER_NONE = 0,
        LIFT_VENDER_WLONG,
        LIFT_VENDER_RILI,
        LIFT_VENDER_SLINGSH,
        LIFT_VENDER_SLING,
    };
    LiftVenderType mVenderType;
private:
    LiftCtrlRequestHandler* getHttpHandler(LiftVenderType vender_type);
    LiftCtrlMqHandler* getMqHandler(LiftVenderType vender_type);
    static void mqRecvTask(void *arg);
public:
    LiftCtrlService();
    ~LiftCtrlService();
    int chooseLiftVender(LiftVenderType vender_type);
};
