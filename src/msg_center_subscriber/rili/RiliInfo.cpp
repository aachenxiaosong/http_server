#include "RiliInfo.hpp"

CJsonObject RiliInfo :: mInfo;

const CJsonObject& RiliInfo :: getInfo() {
    return mInfo;
}

int RiliInfo :: setInfo(const CJsonObject& info) {
    mInfo = info;
    return 0;
}