#ifndef MSG_CENTER_INFO_INIT_INFO_HPP_
#define MSG_CENTER_INFO_INIT_INFO_HPP_

#include "CJsonObject.hpp"
#include "uni_iot.h"

using namespace std;
using namespace neb;

class InitInfo {
    private:
    InitInfo();
    static CJsonObject mInfo;
    static pthread_mutex_t mMutex;

    public:
    static int getInfo(CJsonObject&jinfo);
    static int getBuilding(int building_id, CJsonObject& jbuilding);
    static int getUnit(int unit_id, CJsonObject& junit);
    static int getHome(int home_id, CJsonObject&jhome);
    static int setInfo(const CJsonObject& info);
};

#endif  //  MSG_CENTER_INFO_INIT_INFO_HPP_