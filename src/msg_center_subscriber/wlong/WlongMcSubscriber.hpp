#ifndef MSG_CENTER_SUBSCRIBER_WLONG_WLONG_MC_SUBSCRIBER_H_
#define MSG_CENTER_SUBSCRIBER_WLONG_WLONG_MC_SUBSCRIBER_H_

#include "IMcSubscriber.hpp"
#include "WlongBookLiftMsgHandler.hpp"
#include "WlongInitMsgHandler.hpp"

class WlongMcSubscriber : public IMcSubscriber {
    private:
    WlongBookLiftMsgHandler mBookLiftHandler;
    WlongInitMsgHandler mInitHandler;
    public:
    WlongMcSubscriber();
    int registerMsgHandler();
};

#endif  //  MSG_CENTER_SUBSCRIBER_WLONG_WLONG_MC_SUBSCRIBER_H_
