#include "TcpConn.hpp"
#include "event2/bufferevent.h"
#include <string.h>
#include "uni_log.h"

#define TCP_CONN_TAG mName.c_str()

TcpConn :: TcpConn(const string &ip, uint16_t port, void *socket, vector<ITcpMessageHandler *> &receivers, ITcpPacker *packer) {
    mReceiverLock.writeLock();
    mIp = ip;
    mPort = port;
    mSocket = socket;
    mTag = ip; //tag inited as ip
    mName = "tcp_conn_" + mTag;
    if (packer) {
        mPacker = packer->copy();
    } else {
        mPacker = NULL;
    }
    vector<ITcpMessageHandler *> :: iterator it;
    for (it = receivers.begin(); it != receivers.end(); it++) {
        //ITcpMessageHandler *receiver = (*it)->copy();
        ITcpMessageHandler *receiver = *it;
        receiver->setConn(this);
        mHandlers.push_back(receiver);
    }
    LOGT(TCP_CONN_TAG, "tcp connection created, ip=%s port=%d socket=%p", mIp.c_str(), mPort, mSocket);
    mReceiverLock.writeUnlock();
}

TcpConn :: ~TcpConn() {
    mReceiverLock.writeLock();
    if (mPacker) {
        delete mPacker;
    }
    /*
    vector<ITcpMessageHandler *> :: iterator it;
    for (it = mHandlers.begin(); it != mHandlers.end();) {
        delete *it;
        it = mHandlers.erase(it);
    }*/
    mHandlers.clear();

    LOGT(TCP_CONN_TAG, "tcp connection destroyed, ip=%s port=%d", mIp.c_str(), mPort);
    mReceiverLock.writeUnlock();
}

void TcpConn :: setTag(const string &tag) {
    mTag = tag;
    mName = "tcp_conn_" + mTag;
}

string& TcpConn :: getTag() {
    return mTag;
}

string& TcpConn :: getIp() {
    return mIp;
}

uint16_t TcpConn :: getPort() {
    return mPort;
}

void* TcpConn :: getSocket() {
    return mSocket;
}

//给tcp server调用
int TcpConn :: onRecv(const char *data, int len) {
    int data_received_ok = -1;
    char pack_buf[MAX_TCP_PACK_LEN] = {0};
    int pack_len = MAX_TCP_PACK_LEN;
    ITcpMessageHandler *i_receiver = NULL;
    vector<ITcpMessageHandler *>::iterator it;
    Message *message;
    mReceiverLock.readLock();
    if (mPacker) {
        mPacker->unpackIn(data, len);
        while ((message = mPacker->unpackOut()) != NULL)
        {
            for (vector<ITcpMessageHandler *>::iterator it = mHandlers.begin(); it != mHandlers.end(); it++)
            {
                i_receiver = *it;
                if (i_receiver->handle(*message) == 0)
                {
                    data_received_ok = 0;
                    break;
                }
            }
            delete message;
        }
    }
    mReceiverLock.readUnlock();
    return data_received_ok;
}

//给app调用
int TcpConn :: send(const char *data, int len) {
    LOGT(TCP_CONN_TAG, "send data len %d", len);
    return bufferevent_write((struct bufferevent *)mSocket, data, len);
}
