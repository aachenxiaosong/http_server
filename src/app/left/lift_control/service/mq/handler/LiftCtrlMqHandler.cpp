#include "LiftCtrlMqHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "UniLog.hpp"

#define LIFT_CTRL_MQ_HANDLER_TAG mName.c_str()

int LiftCtrlMqHandler :: addMessageHandler(ILiftCtrlMessageHandler *handler) {
    mMessageHandlers.push_back(handler);
    return 0;
}

string LiftCtrlMqHandler :: failureResponse(int ret_code, string msg) {
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

int LiftCtrlMqHandler :: handle(MqTopicType topic, const string& request, string& response) {
    int ret = -1;
    //step 1: check init info
    if (SulinkLiftInitData::inited() != true) {
        response = failureResponse(-1, "request rejected for init info is absent");
        LOGE(LIFT_CTRL_MQ_HANDLER_TAG, "Mq request topic %d is rejected for init info is absent", topic);
        return 0;
    }
    LOGT(LIFT_CTRL_MQ_HANDLER_TAG, "Mq request topic %d is handled", topic);
    //step 2: unpack
    LiftCtrlMessageReq *m_req = mPacker.unpack(topic, request);
    if (m_req == NULL) {
        response = failureResponse(-1, "parse Mq failed");
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