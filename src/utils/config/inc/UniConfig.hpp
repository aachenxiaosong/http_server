#pragma once

#include "Poco/Util/IniFileConfiguration.h"
#include <string>

namespace unisound {
    class Config {
    private:
        std::string mPath;
        Poco::Util::IniFileConfiguration *mConf;
    public:
        Config(const std::string& path);
        std::string getString(const std::string& key);
        bool getBool(const std::string& key);
        int getInt(const std::string& key);
        double getDouble(const std::string& key);
    };

    class UniConfig {
    private:
        static Config mConfig;
    public:
        static std::string getString(const std::string& key);
        static bool getBool(const std::string& key);
        static int getInt(const std::string& key);
        static double getDouble(const std::string& key);
    };
}
