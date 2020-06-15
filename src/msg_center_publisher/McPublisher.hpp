#ifndef MSG_CENTER_PUBLISHER_MC_PUBLISHER_H_
#define MSG_CENTER_PUBLISHER_MC_PUBLISHER_H_

#include <string>
#include "CJsonObject.hpp"

using namespace std;
using namespace neb;

class McPublisher{
    private:
    string name;
    public:
    McPublisher(const char* name) ;
};

#endif  //  MSG_CENTER_PUBLISHER_MC_PUBLISHER_H_
