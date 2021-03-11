#include "Sulink.hpp"
#include "SulinkLiftInitData.hpp"
#include "HttpServer.hpp"
#include <iostream>

Sulink :: Sulink() : mSulinkHttpHandler("sulink_http_handler")
{
    mSulinkHttpHandler.addMessageHandler(&mRecvLiftInfoHandler);
    //mSulinkHttpHandler.addMessageHandler(&mRecvLiftCtrlHandler);
    //mSulinkHttpHandler.addMessageHandler(&mRecvLiftStatusHandler);
    mSulinkHttpHandler.setPacker(&mPacker);
    

    http_server_add_handler(&mSulinkHttpHandler);
}

int Sulink :: init()
{
    SulinkLiftInitData::loadFromConfig();
   
    return 0;
}