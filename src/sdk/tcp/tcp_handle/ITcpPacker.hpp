#ifndef  SDK_TCP_TCP_HANDLE_ITCP_PACKER_HPP_
#define  SDK_TCP_TCP_HANDLE_ITCP_PACKER_HPP_

#include <string>
#include <vector>
#include "Message.hpp"
#include "TcpEnum.hpp"

using namespace std;


/* 可以仅仅做分包(防止粘包或半包),也可以深层次地转数据结构 */
class ITcpPacker {
private:
    string mName;
public:
    ITcpPacker(const char *name);
    string getName();
    virtual ~ITcpPacker();
    //0=pack ok, -1=pack failed
    virtual int unpackIn(const char *raw_data, int raw_data_len) = 0;
    //0=pack has output, -1=pack has no output
    virtual Message* unpackOut() = 0;
    //packer会为每个conn拷贝一份,因为每个conn要独立缓存中间处理数据
    virtual ITcpPacker *copy() = 0;
};

#endif  //  SDK_TCP_TCP_HANDLE_ITCP_PACKER_HPP_