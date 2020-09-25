#ifndef SERVER_LIFT_CONTROL_RILI_MQTT_RILI_MC_SUBSCRIBER_H_
#define SERVER_LIFT_CONTROL_RILI_MQTT_RILI_MC_SUBSCRIBER_H_

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

#endif  //  SERVER_LIFT_CONTROL_RILI_MQTT_RILI_MC_SUBSCRIBER_H_