#ifndef  SDK_PROTOCOL_PACKER_IPROTOCOL_PACKER_HPP_
#define  SDK_PROTOCOL_PACKER_IPROTOCOL_PACKER_HPP_

#include <string>
#include <vector>

using namespace std;

/* 可以仅仅做分包(防止粘包或半包),也可以深层次地转数据结构 */
class IProtocolPacker {
private:
    string mName;
public:
    IProtocolPacker(const char *name);
    string getName();
    virtual ~IProtocolPacker();
    //0=pack ok, -1=pack failed
    virtual int pack(const char *raw_data, int raw_data_len, char *packed_data, int* packed_data_len) = 0;
    //0=unpack ok, -1=unpack failed
    virtual int unpack(const char *packed_data, int packed_data_len, char *raw_data, int* raw_data_len) = 0;
};  

#endif  //  SDK_PROTOCOL_PACKER_IPROTOCOL_PACKER_HPP_