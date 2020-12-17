#pragma once

#include <sstream>

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "UniLog.hpp"

#define SERIALIZATION_TAG "serialization"

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
        T t;
        try {
            stringstream ss(str);
            archive::text_iarchive(ss) >> t;
        } catch (archive::archive_exception e) {
            LOGE(SERIALIZATION_TAG, "deserialization exception: %s", e.what());
        }
        return t;
    }
};

}

