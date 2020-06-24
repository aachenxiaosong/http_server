#ifndef MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_INIT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_INIT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"
#include "IHttpService.hpp"

using namespace neb;


class WlongInitMsgHandler : public IMcMsgHandler {
    private:
    IHttpService *mHttpService;
    private:
    int _save_init_info(string &info);
    int _import_init_info(string &info);
    int _parse_home(CJsonObject& djson, CJsonObject& sjson) ;
    int _parse_unit(CJsonObject& djson, CJsonObject& sjson);
    int _parse_building(CJsonObject& djson, CJsonObject& sjson);
    int _parse(CJsonObject& djson, CJsonObject& sjson);
    public:
    WlongInitMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_HANDLERS_WLONG_WLONG_INIT_MSG_HANDLER_H_
