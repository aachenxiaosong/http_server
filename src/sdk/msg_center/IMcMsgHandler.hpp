#ifndef SDK_MSG_CENTER_I_MC_MSG_HANDLER_H_
#define SDK_MSG_CENTER_I_MC_MSG_HANDLER_H_

#include <string>
using namespace std;

class IMcMsgHandler {
    private:
    string mName;
    public:
    IMcMsgHandler(const char* name) ;
    virtual ~IMcMsgHandler() ;
    string getName(); 
    virtual int handle(string& topic, string& msg) = 0;
};

#endif