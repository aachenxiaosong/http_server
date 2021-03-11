#include "SulinkRequestHandler.hpp"
#include "LiftCtrlRequestHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "UniLog.hpp"

#define SULINK_REQ_HANDLER_TAG getName().c_str()

int SulinkRequestHandler :: setPacker(ISulinkPacker *packer) {
    mPacker = packer;
    return 0;
}

int SulinkRequestHandler :: addMessageHandler(ISulinkMessageHandler *handler) {
    mHandlers.push_back(handler);
    return 0;
}

int SulinkRequestHandler :: handle(const string& path, const string& request, string& response) {
    int ret = -1;
    ISulinkMessageHandler *i_handler = NULL;
    

    LOGT(SULINK_REQ_HANDLER_TAG, "request %s is handled", path.c_str());
    //if (path.compare("/lift/ipc/forward") == 0) {
    //    return 0;
    //}
    
    //step 1: unpack
    ISulinkMessage *http_message = mPacker->unpack(request);
    if (http_message)
    {
        ISulinkMessage *http_ack;
        http_message->topic("sulink.message");
        for (vector<ISulinkMessageHandler *>::iterator it = mHandlers.begin(); it != mHandlers.end(); it++)
        {
            i_handler = *it;
            http_ack = (ISulinkMessage *)i_handler->handle(*http_message);
            if (http_ack != NULL)
            {
                string* data = mPacker->pack(*http_ack);
                if (data) {
                    response = *data;
                    ret = 0;
                    delete data;
                }
                delete http_ack;
                break;
            }
        }
        delete http_message;
    }
    
    return ret;
}

