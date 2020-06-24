#include "InitInfo.hpp"

CJsonObject InitInfo :: mInfo;

const CJsonObject& InitInfo :: getInfo() {
    return mInfo;
}

int InitInfo :: setInfo(const CJsonObject& info) {
    mInfo = info;
    return 0;
}