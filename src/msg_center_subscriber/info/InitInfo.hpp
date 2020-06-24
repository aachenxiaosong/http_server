#ifndef MSG_CENTER_INFO_INIT_INFO_HPP_
#define MSG_CENTER_INFO_INIT_INFO_HPP_

#include "CJsonObject.hpp"

using namespace neb;

class InitInfo {
    private:
    InitInfo();
    static CJsonObject mInfo;

    public:
    static const CJsonObject& getInfo();
    static int setInfo(const CJsonObject& info);
};

#endif  //  MSG_CENTER_INFO_INIT_INFO_HPP_