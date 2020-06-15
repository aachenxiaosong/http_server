#include "WlongInfo.hpp"

CJsonObject WlongInfo :: mInfo;

const CJsonObject& WlongInfo :: getInfo() {
    return mInfo;
}

int WlongInfo :: setInfo(const CJsonObject& info) {
    mInfo = info;
    return 0;
}