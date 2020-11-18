#include "LiftCtrlService.hpp"
#include "HttpServer.hpp"

LiftCtrlService :: LiftCtrlService()
{
    mWlongHandler.addMessageHandler(&mWlongBookLiftHandler);
    mWlongHandler.addMessageHandler(&mWlongCallLiftHandler);
    mWlongHandler.addMessageHandler(&mWlongBookLiftInterHandler);
    mWlongHandler.addMessageHandler(&mWlongTakeLiftHandler);
    mVenderType = LIFT_VENDER_NONE;
}

LiftCtrlService :: ~LiftCtrlService()
{}

LiftCtrlRequestHandler* LiftCtrlService :: getHandler(LiftVenderType vender_type)
{
    LiftCtrlRequestHandler* handler = NULL;
    switch (vender_type) {
        case LIFT_VENDER_WLONG:
            handler = &mWlongHandler;
            break;
        default:
            break;
    }
    return handler;
}


int LiftCtrlService :: chooseLiftVender(LiftVenderType vender_type)
{
    int ret = -1;
    IHttpRequestHandler *handler = NULL;;
    handler = getHandler(mVenderType);
    if (handler != NULL) {
        http_server_del_handler(handler);
    }
    handler = getHandler(vender_type);
    if (handler != NULL) {
        http_server_add_handler(handler);
        mVenderType = vender_type;
        ret = 0;
    } else {
        mVenderType = LIFT_VENDER_NONE;
    }
    return ret;
}