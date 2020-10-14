#include "ITcpPacker.hpp"
#include "uni_log.h"

string ITcpPacker :: getName() {
    return mName;
}

ITcpPacker :: ITcpPacker(const char *name) {
    mName = name;
}

ITcpPacker :: ~ITcpPacker() {
}