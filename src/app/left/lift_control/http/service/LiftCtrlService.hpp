#ifndef LEFT_LIFT_CONTROL_SERVICE_LIFT_CTRL_SERVICE_HPP_
#define LEFT_LIFT_CONTROL_SERVICE_LIFT_CTRL_SERVICE_HPP_

#include "LiftCtrlRequestHandler.hpp"
#include "WlongBookLiftMessageHandler.hpp"
#include "WlongCallLiftMessageHandler.hpp"
#include "WlongBookLiftInterMessageHandler.hpp"
#include "WlongTakeLiftMessageHandler.hpp"


class LiftCtrlService {
private:
    WlongBookLiftMessageHandler mWlongBookLiftHandler;
    WlongCallLiftMessageHandler mWlongCallLiftHandler;
    WlongBookLiftInterMessageHandler mWlongBookLiftInterHandler;
    WlongTakeLiftMessageHandler mWlongTakeLiftHandler;
    LiftCtrlRequestHandler mWlongHandler;
public:
    enum LiftVenderType {
        LIFT_VENDER_NONE = 0,
        LIFT_VENDER_WLONG
    };
    LiftVenderType mVenderType;
private:
    LiftCtrlRequestHandler* getHandler(LiftVenderType vender_type);
public:
    LiftCtrlService();
    ~LiftCtrlService();
    int chooseLiftVender(LiftVenderType vender_type);
};

#endif  //  LEFT_LIFT_CONTROL_SERVICE_LIFT_CTRL_SERVICE_HPP_