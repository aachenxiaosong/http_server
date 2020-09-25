#include "RiliMcSubscriber.hpp"

#include "IMcSubscriber.hpp"
#include "McBroker.hpp"
#include "uni_log.h"

#define RILI_MC_SUBSCRIBER "rili_mc_subscriber"

RiliMcSubscriber :: RiliMcSubscriber() : IMcSubscriber("rili_mc_subscriber") {
    LOGT(RILI_MC_SUBSCRIBER, "RiliMcSubscriber is created");
}

int RiliMcSubscriber :: registerMsgHandler() {
    IMcMsgHandler *handler;
    //handler = &mBookLiftHandler;
    //McBrokerAddHandler(handler);
    handler = &mInitHandler;
    McBrokerAddHandler(handler);
    return 0;
}