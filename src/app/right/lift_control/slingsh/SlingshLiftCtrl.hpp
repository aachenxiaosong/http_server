#pragma once
#include <string>

using namespace std;

class SlingshLiftCtrl {
private:
    string mIp;
    int mPort;
private:
    int tcpSend(unsigned char *request, int request_len, unsigned char *response, int *response_len);
public:
    SlingshLiftCtrl(const string& url);
    int bookElevator(const string& from_floor, const string& to_floor);
};