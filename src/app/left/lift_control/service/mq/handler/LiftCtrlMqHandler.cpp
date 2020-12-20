#include "LiftCtrlMqHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "UniLog.hpp"

#define LIFT_CTRL_MQ_HANDLER_TAG mName.c_str()

int LiftCtrlMqHandler :: addMessageHandler(ILiftCtrlMqMessageHandler *handler) {
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

int LiftCtrlMqHandler :: handle(MqTopicType topic, const string& request) {
    int ret = -1;
    //step 1: check init info
    if (SulinkLiftInitData::inited() != true) {
        LOGE(LIFT_CTRL_MQ_HANDLER_TAG, "Mq request of topic %d is rejected for init info is absent", topic);
        return 0;
    }
    LOGT(LIFT_CTRL_MQ_HANDLER_TAG, "Mq request of topic %d is received", topic);
    //step 2: unpack
    LiftCtrlMessageReq *m_req = mPacker.unpack(topic, request);
    if (m_req == NULL || m_req->retcode() != 0) {
        LOGE(LIFT_CTRL_MQ_HANDLER_TAG, "Mq request of topic %d is parsed failed, msg=%s", topic, m_req->msg().c_str());
        return 0;
    }
    //step 3: process request
    if (m_req != NULL)
    {
        for (int i = 0; i < mMessageHandlers.size(); i++)
        {
            if (0 == mMessageHandlers[i]->handle(*m_req))
            {
                LOGT(LIFT_CTRL_MQ_HANDLER_TAG, "Mq request of topic %d is handled OK", topic);
                ret = 0;
                break;
            }
        }
        delete m_req;
    }
    return ret;
}
