#ifndef MSG_CENTER_SUBSCRIBER_SUBSCRIBTERS_RILI_RILI_MC_SUBSCRIBER_H_
#define MSG_CENTER_SUBSCRIBER_SUBSCRIBTERS_RILI_RILI_MC_SUBSCRIBER_H_

#include "IMcSubscriber.hpp"
#include "RiliBookLiftMsgHandler.hpp"
#include "RiliInitMsgHandler.hpp"

class RiliMcSubscriber : public IMcSubscriber {
    private:
    RiliBookLiftMsgHandler mBookLiftHandler;
    RiliInitMsgHandler mInitHandler;
    public:
    RiliMcSubscriber();
    int registerMsgHandler();
};

#endif  //  MSG_CENTER_SUBSCRIBER_SUBSCRIBTERS_RILI_RILI_MC_SUBSCRIBER_H_
