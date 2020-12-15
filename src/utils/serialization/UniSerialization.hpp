#pragma once

#include <sstream>

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/vector.hpp"

using namespace boost;

namespace unisound {
template <typename T>
class UniSerialization {
public:
    static std::string seri(const T& t) {
        stringstream ss;
        archive::text_oarchive(ss) << t;
        return ss.str();
    }

    static T deseri(const string& str) {
        stringstream ss(str);
        T t;
        archive::text_iarchive(ss) >> t;
        return t;
    }
};

}

