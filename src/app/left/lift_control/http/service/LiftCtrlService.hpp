#pragma once

#include "LiftCtrlRequestHandler.hpp"
#include "WlongBookLiftMessageHandler.hpp"
#include "WlongCallLiftMessageHandler.hpp"
#include "WlongBookLiftInterMessageHandler.hpp"
#include "WlongTakeLiftMessageHandler.hpp"
#include "WlongLiftStatusMessageHandler.hpp"
#include "RiliBookLiftMessageHandler.hpp"
#include "RiliCallLiftMessageHandler.hpp"
#include "RiliBookLiftInterMessageHandler.hpp"
#include "RiliTakeLiftMessageHandler.hpp"
#include "RiliLiftStatusMessageHandler.hpp"
#include "Mq.hpp"
#include <thread>


class LiftCtrlService {
private:
    Mq *mMq;
    thread *mMqThread;
    bool mMqThreadRunning;
   //wanglong message handlers
    WlongBookLiftMessageHandler mWlongBookLiftHandler;
    WlongCallLiftMessageHandler mWlongCallLiftHandler;
    WlongBookLiftInterMessageHandler mWlongBookLiftInterHandler;
    WlongTakeLiftMessageHandler mWlongTakeLiftHandler;
    WlongLiftStatusMessageHandler mWlongLiftStatusHandler;
    LiftCtrlRequestHandler mWlongHandler;
    //rili message handlers
    RiliBookLiftMessageHandler mRiliBookLiftHandler;
    RiliCallLiftMessageHandler mRiliCallLiftHandler;
    RiliBookLiftInterMessageHandler mRiliBookLiftInterHandler;
    RiliTakeLiftMessageHandler mRiliTakeLiftHandler;
    RiliLiftStatusMessageHandler mRiliLiftStatusHandler;
    LiftCtrlRequestHandler mRiliHandler;
public:
    enum LiftVenderType {
        LIFT_VENDER_NONE = 0,
        LIFT_VENDER_WLONG,
        LIFT_VENDER_RILI,
    };
    LiftVenderType mVenderType;
private:
    LiftCtrlRequestHandler* getHandler(LiftVenderType vender_type);
    static void mqRecvTask(void *arg);
public:
    LiftCtrlService();
    ~LiftCtrlService();
    int chooseLiftVender(LiftVenderType vender_type);
};