#include "IProtocolPacker.hpp"
#include "uni_log.h"

string IProtocolPacker :: getName() {
    return mName;
}

IProtocolPacker :: IProtocolPacker(const char *name) {
    mName = name;
}

IProtocolPacker :: ~IProtocolPacker() {
}