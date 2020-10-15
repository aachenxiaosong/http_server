#ifndef APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_HPP_
#define APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_HPP_


#include "DechangServer.hpp"
#include "Message.hpp"

#include <string>

using namespace std;

class Dechang {
private:
    DechangServer server;
public:
    void init();
    //TODO: 不应该在这里,也应该是receiver,这个receiver里注册一个event处理事件
    //每个Conn下有个eventRoute,或dispather,然后receiver向这个eventRoute注册,这样找起来就很快
    //这里用dispatcher应该比较合适
    int onRecvMessage(TcpConn *conn, Message &message);
};

#endif  //  APP_DOWN_CHILD_DEVICE_DECHANG_DECHANG_HPP_