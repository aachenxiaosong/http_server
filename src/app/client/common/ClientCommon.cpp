#include "ClientCommon.hpp"
#include "HttpAuth.hpp"
#include "HttpHb.hpp"

int ClientCommon::init() {
    if (HttpAuth::init() != 0) {
        return -1;
    }
    if (HttpHb::init() != 0) {
        return -1;
    }
    return 0;
}