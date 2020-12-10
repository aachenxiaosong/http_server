#include "LiftCtrl.hpp"
#include "SulinkLiftInitData.hpp"

int LiftCtrl :: start()
{
    string brand_code = SulinkLiftInitData::getBrandCode();
    LiftCtrlService::LiftVenderType vender_type = LiftCtrlService::LiftVenderType::LIFT_VENDER_NONE;
    if (brand_code.compare("1") == 0) {
        vender_type = LiftCtrlService::LiftVenderType::LIFT_VENDER_WLONG;
    } else if (brand_code.compare("2") == 0) {
        vender_type = LiftCtrlService::LiftVenderType::LIFT_VENDER_RILI;
    }
    mService.chooseLiftVender(vender_type);
    return 0;
}
