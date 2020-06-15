#include "WlongMcSubscriber.hpp"

#include "IMcSubscriber.hpp"
#include "McBroker.hpp"
#include "uni_log.h"

#define WLONG_MC_SUBSCRIBER "wlong_mc_subscriber"

WlongMcSubscriber :: WlongMcSubscriber() : IMcSubscriber("wlong_mc_subscriber") {
    LOGT(WLONG_MC_SUBSCRIBER, "WlongMcSubscriber is created");
}

int WlongMcSubscriber :: registerMsgHandler() {
    IMcMsgHandler *handler;
    handler = &mBookLiftHandler;
    McBrokerAddHandler(handler);
    handler = &mInitHandler;
    McBrokerAddHandler(handler);
    return 0;
}