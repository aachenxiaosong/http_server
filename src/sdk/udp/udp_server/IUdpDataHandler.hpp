#pragma once
#include <string>


class IUdpDataHandler {
private:
    std::string mName;
public:
    IUdpDataHandler(const char* name) {
        mName = name;
    }
    std::string getName() {
        return mName;
    }
    virtual int handle(const char* data, int data_len) = 0;
};