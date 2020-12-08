#include "UniDeviceInfo.hpp"
#include "UniConfig.hpp"
#include "UniLog.hpp"
#include "uni_iot.h"

#define DEVICE_INFO_TAG "device_info"

std::string unisound::UniDeviceInfo ::mMac;
std::string unisound::UniDeviceInfo ::mUdid;
std::string unisound::UniDeviceInfo ::mType;
std::string unisound::UniDeviceInfo ::mNetName;
std::string unisound::UniDeviceInfo ::mAppVersion;

std::string unisound::UniDeviceInfo ::getMac()
{
    
#ifdef IS_VMWARE
    return "7c:cb:e2:e2:97:74";
#else
    if (mMac.empty() == true)
    {
        char mac[64] = {0};
        int sock, ret;
        struct ifreq ifr;
        memset(&ifr, 0, sizeof(ifr));
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            LOGE(DEVICE_INFO_TAG, "socket create failed");
            return mMac;
        }
        strcpy(ifr.ifr_name, getNetName().c_str());
        if ((ret = ioctl(sock, SIOCGIFHWADDR, &ifr, sizeof(ifr))) == 0)
        {
            snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
                     ifr.ifr_hwaddr.sa_data[0] & 0xff,
                     ifr.ifr_hwaddr.sa_data[1] & 0xff,
                     ifr.ifr_hwaddr.sa_data[2] & 0xff,
                     ifr.ifr_hwaddr.sa_data[3] & 0xff,
                     ifr.ifr_hwaddr.sa_data[4] & 0xff,
                     ifr.ifr_hwaddr.sa_data[5] & 0xff);
            mMac = mac;
            LOGT(DEVICE_INFO_TAG, "get device mac %s", mac);
        }
        else
        {
            LOGE(DEVICE_INFO_TAG, "get mac failed");
        }
        close(sock);
    }
    return mMac;
#endif
}

std::string unisound::UniDeviceInfo ::getUdid()
{
#ifdef IS_VMWARE
    return "7ccbe2e29774";
#else
    if (mUdid.empty() == true)
    {
        if (mMac.empty() != true)
        {
            mUdid = mMac;
            mUdid.erase(2, 1).erase(4, 1).erase(6, 1).erase(8, 1).erase(10, 1);
            LOGT(DEVICE_INFO_TAG, "get device udid %s", mUdid.c_str());
        }
    }
    return mUdid;
#endif
}

std::string unisound::UniDeviceInfo ::getType()
{
    if (mType.empty() == true) {
        mType = UniConfig::getString("device.type");
    }
    return mType;
}

std::string unisound::UniDeviceInfo ::getNetName()
{
    if (mNetName.empty() == true) {
        mNetName = UniConfig::getString("device.netname");
    }
    return mNetName;
}

std::string unisound::UniDeviceInfo ::getAppVersion()
{
    if (mAppVersion.empty() == true) {
        mAppVersion = UniConfig::getString("device.version.app");
    }
    return mAppVersion;
}


//通过获取/proc/stat (CPU)和/proc/meminfo(内存和硬盘)文件信息
//proc/meminfo文件结构
//MemTotal:        515164 kB
//MemFree:         15024 kB
//MemAvailable:    15024 kB
//Buffers:         13740 kB
//Cached:          248824 kB
//SwapCached:      960 kB
//Active:          309980 kB
//Inactive:        63420 kB
typedef struct MEMPACKED //定义一个mem occupy的结构体
{
    char name1[20]; //定义一个char类型的数组名name有20个元素
    unsigned long MemTotal;
    char name2[20];
    unsigned long MemFree;
    char name3[20];
    unsigned long MemAvailable;
    char name4[20];
    unsigned long Buffers;
    char name5[20];
    unsigned long Cached;
    char name6[20];
    unsigned long SwapCached;
} MEM_OCCUPY;

//proc/stat文件结构
//cpu  633666 46912 249878 176813696 782884 2859 19625 0
//cpu0 633666 46912 249878 176813696 782884 2859 19625 0
//intr 5812844
//ctxt 265816063
//btime 1455203832
//processes 596625
//procs_running 1
//procs_blocked 0

typedef struct CPUPACKED //定义一个cpu occupy的结构体
{
    char name[20];       //定义一个char类型的数组名name有20个元素
    unsigned int user;   //定义一个无符号的int类型的user
    unsigned int nice;   //定义一个无符号的int类型的nice
    unsigned int system; //定义一个无符号的int类型的system
    unsigned int idle;   //定义一个无符号的int类型的idle
    unsigned int lowait;
    unsigned int irq;
    unsigned int softirq;
} CPU_OCCUPY;

double unisound::UniDeviceInfo ::getCpuUtility()
{
    FILE *fd;
    char buff[256];
    CPU_OCCUPY cpu_occupy;
    fd = fopen("/proc/stat", "r");
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %u %u %u %u %u %u %u", cpu_occupy.name, &cpu_occupy.user, &cpu_occupy.nice, &cpu_occupy.system, &cpu_occupy.idle, &cpu_occupy.lowait, &cpu_occupy.irq, &cpu_occupy.softirq);
    fclose(fd);
    unsigned long total = (unsigned long)(cpu_occupy.user + cpu_occupy.nice + cpu_occupy.system + cpu_occupy.idle + cpu_occupy.lowait + cpu_occupy.irq + cpu_occupy.softirq);
    double utility = (double)(total - cpu_occupy.idle) / (double)total;
    return utility;
}

double unisound::UniDeviceInfo ::getMemUtility()
{
    FILE *fd;
    char buff[256];
    MEM_OCCUPY m;
    fd = fopen("/proc/meminfo", "r");
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m.name1, &m.MemTotal);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m.name2, &m.MemFree);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m.name3, &m.MemAvailable);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m.name4, &m.Buffers);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m.name5, &m.Cached);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m.name6, &m.SwapCached);
    fclose(fd); //关闭文件fd
    double utility = (double)(m.MemTotal - m.MemAvailable) / (double)m.MemTotal;
    return utility;
}
