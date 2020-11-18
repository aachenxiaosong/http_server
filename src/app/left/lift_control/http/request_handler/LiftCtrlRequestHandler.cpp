#include "LiftCtrlRequestHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "uni_log.h"

#define LIFT_CTRL_REQ_HANDLER_TAG "lift_ctrl_req_handler"

int LiftCtrlRequestHandler :: addMessageHandler(ILiftCtrlMessageHandler *handler) {
    mMessageHandlers.push_back(handler);
    return 0;
}

string LiftCtrlRequestHandler :: failureResponse(int ret_code, string msg) {
    string ret = "";
    LiftCtrlMessageRsp m_rsp;
    m_rsp.retcode(ret_code);
    m_rsp.msg(msg);
    string *rsp = mPacker.pack(m_rsp);
    if (NULL != rsp)
    {
        ret = *rsp;
        delete rsp;
    }
    return ret;
}

int LiftCtrlRequestHandler :: handle(const string& path, const string& request, string& response) {
    int ret = -1;
    //step 1: check init info
    if (SulinkLiftInitData::inited() != true) {
        response = failureResponse(-1, "request rejected for init info is absent");
        return 0;
    }
    //step 2: unpack
    LiftCtrlMessageReq *m_req = mPacker.unpack(path, request);
    if (m_req == NULL) {
        response = failureResponse(-1, "parse request failed");
        return 0;
    }
    if (m_req->retcode() != 0) {
        response = failureResponse(m_req->retcode(), m_req->msg());
        delete m_req;
        return 0;
    }
    //step 3: process request
    if (m_req != NULL)
    {
        LiftCtrlMessageRsp *m_rsp = NULL;
        for (int i = 0; i < mMessageHandlers.size(); i++)
        {
            m_rsp = mMessageHandlers[i]->handle(*m_req);
            if (NULL != m_rsp)
            {
                LOGT(LIFT_CTRL_REQ_HANDLER_TAG, "request is handled by %s", mMessageHandlers[i]->getName().c_str());
                string *rsp = mPacker.pack(*m_rsp);
                if (NULL != rsp)
                {
                    response = *rsp;
                    ret = 0;
                    delete rsp;
                }
                delete m_rsp;
                break;
            }
        }
        delete m_req;
    }
    return ret;
}