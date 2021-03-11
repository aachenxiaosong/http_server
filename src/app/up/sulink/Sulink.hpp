#ifndef APP_UP_SULINK_SULINK_HPP_
#define APP_UP_SULINK_SULINK_HPP_

#include "SulinkRequestHandler.hpp"
#include "SulinkPacker.hpp"
#include "SulinkRecvLiftInfoHandler.hpp"
//#include "SulinkRecvLiftCtrlHandler.hpp"
//#include "SulinkRecvLiftStatusHandler.hpp"

class Sulink {
private:
    SulinkPacker mPacker;
    SulinkRecvLiftInfoHandler mRecvLiftInfoHandler;
    //SulinkRecvLiftCtrlHandler mRecvLiftCtrlHandler;
    //SulinkRecvLiftStatusHandler mRecvLiftStatusHandler;
    
    SulinkRequestHandler mSulinkHttpHandler;
public:
    Sulink();
    ~Sulink(){}
    int init();
};

#endif  //  APP_UP_SULINK_SULINK_HPP_