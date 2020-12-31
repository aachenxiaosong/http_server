#pragma once
#include <string>

namespace unisound {
    class UniDeviceInfo {
    private:
        static std::string mMac;
        static std::string mUdid;
        static std::string mType;
        static std::string mNetName;
        static std::string mAppVersion;
    public:
        static std::string getMac();
        static std::string getIp();
        static std::string getUdid();
        static std::string getType();
        static std::string getNetName();
        static std::string getAppVersion();
        static double getCpuUtility();
        static double getMemUtility();
    };
}
