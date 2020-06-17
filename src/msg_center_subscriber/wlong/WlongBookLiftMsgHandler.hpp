#ifndef MSG_CENTER_SUBSCRIBER_WLONG_WLONG_BOOK_LIFT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_WLONG_WLONG_BOOK_LIFT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"

using namespace neb;


class WlongBookLiftMsgHandler : public IMcMsgHandler {
    private:
    int _checkBookLift(CJsonObject& msg, string& err_field) ;
    int _handleBookLift(CJsonObject& msg, CJsonObject& jresponse) ;
    public:
    WlongBookLiftMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_WLONG_WLONG_BOOK_LIFT_MSG_HANDLER_H_
