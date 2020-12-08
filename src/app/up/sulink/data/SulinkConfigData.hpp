#pragma once
#include "UniConfig.hpp"
#include <string>

using namespace std;
using namespace unisound;

class SulinkConfigData {
public:
    static string getCloudType() {
        static string cloud_type;
        if (cloud_type.empty()) {
            cloud_type = UniConfig::getString("sulink.could.type");
        }
        return cloud_type;
    }
    static string getAccessKey() {
        static string access_key;
        if (access_key.empty()) {
            access_key = UniConfig::getString("sulink.accesskey");
        }
        return access_key;
    }
    static string getSecretKey() {
        static string secret_key;
        if (secret_key.empty()) {
            secret_key = UniConfig::getString("sulink.secretkey");
        }
        return secret_key;
    }
    static string getBrand() {
        static string brand;
        if (brand.empty()) {
            brand = UniConfig::getString("sulink.brand");
        }
        return brand;
    }
    static int getPayloadVersion() {
        static int payload_version = -1;
        if (payload_version == -1) {
            payload_version = UniConfig::getInt("sulink.payloadversion");
        }
        return payload_version;
    }
    static string getServerDomain() {
        static string server_domain;
        if (server_domain.empty()) {
            server_domain = UniConfig::getString("sulink.server_domain");
        }
        return server_domain;
    }
    static string getUrlDeviceRegister() {
        static string url_device_register;
        if (url_device_register.empty()) {
            url_device_register = getServerDomain() + UniConfig::getString("sulink.url.deviceregister");
        }
        return url_device_register;
    }
    static string getUrlHb() {
        static string url_hb;
        if (url_hb.empty()) {
            url_hb = getServerDomain() + UniConfig::getString("sulink.url.hb");
        }
        return url_hb;
    }
    static string getUrlTimeSync() {
        static string url_time_sync;
        if (url_time_sync.empty()) {
            url_time_sync = getServerDomain() + UniConfig::getString("sulink.url.timesync");
        }
        return url_time_sync;
    }
};