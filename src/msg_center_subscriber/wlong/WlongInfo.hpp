#ifndef MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INFO_HPP_
#define MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INFO_HPP_

#include "CJsonObject.hpp"

using namespace neb;

class WlongInfo {
    private:
    WlongInfo();
    static CJsonObject mInfo;

    public:
    static const CJsonObject& getInfo();
    static int setInfo(const CJsonObject& info);
};

#endif  //  MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INFO_HPP_