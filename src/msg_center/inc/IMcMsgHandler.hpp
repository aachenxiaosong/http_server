#ifndef MSG_CENTER_I_MC_MSG_HANDLER_H_
#define MSG_CENTER_I_MC_MSG_HANDLER_H_

#include <string>
using namespace std;

class IMcMsgHandler {
    private:
    string mName;
    public:
    IMcMsgHandler(const char* name) ;
    string getName(); 
    virtual int handle(string& msg) = 0;
};

#endif