#ifndef MSG_CENTER_SUBSCRIBER_RILI_RILI_INFO_HPP_
#define MSG_CENTER_SUBSCRIBER_RILI_RILI_INFO_HPP_

#include "CJsonObject.hpp"

using namespace neb;

class RiliInfo {
    private:
    RiliInfo();
    static CJsonObject mInfo;

    public:
    static const CJsonObject& getInfo();
    static int setInfo(const CJsonObject& info);
};

#endif  //  MSG_CENTER_SUBSCRIBER_RILI_RILI_INFO_HPP_