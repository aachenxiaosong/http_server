#ifndef APP_LEFT_LIFT_CTRL_LIFT_CTRL_HPP_
#define APP_LEFT_LIFT_CTRL_LIFT_CTRL_HPP_

#include "LiftCtrlService.hpp"

class LiftCtrl {
private:
    LiftCtrlService mService;
public:
    LiftCtrl() = default;
    ~LiftCtrl() = default;
    int start();
};

#endif  //  APP_LEFT_LIFT_CTRL_LIFT_CTRL_HPP_