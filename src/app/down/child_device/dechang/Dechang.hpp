#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_HPP_


#include "DechangServer.hpp"
#include "Message.hpp"

#include <string>

using namespace std;

class Dechang {
private:
    DechangServer mServer;
public:
    int init();
    //message里应该携带conn的tag信息,用于确定message发给那个连接
    int onRecvMessage(const string& conn_tag, const Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_HPP_