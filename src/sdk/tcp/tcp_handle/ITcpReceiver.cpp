#include "ITcpReceiver.hpp"

ITcpReceiver :: ITcpReceiver(const char* name) {
    this->mName = name;
}

ITcpReceiver :: ~ITcpReceiver() {
    
}

string ITcpReceiver :: getName() {
    return mName;
}

void ITcpReceiver :: setConn(TcpConn *conn) {
    mConn = conn;
}
