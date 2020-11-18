#include "LiftCtrl.hpp"

int LiftCtrl :: start()
{
    mService.chooseLiftVender(mService.LIFT_VENDER_WLONG);
    return 0;
}
