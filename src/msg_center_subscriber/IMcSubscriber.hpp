#ifndef MC_SUBSCRIBTER_I_MC_SUBSCRIBTER_H_
#define MC_SUBSCRIBTER_I_MC_SUBSCRIBTER_H_

#include "IMcMsgHandler.hpp"

class IMcSubscriber{
    private:
    string name;
    public:
    IMcSubscriber(const char* name) ;
    virtual ~IMcSubscriber() ;
    virtual int registerMsgHandler() = 0;
};

#endif  //  MC_SUBSCRIBTER_I_MC_SUBSCRIBTER_H_