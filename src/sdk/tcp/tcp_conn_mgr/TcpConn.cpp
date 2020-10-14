#include "TcpConn.hpp"
#include "event2/bufferevent.h"
#include <string.h>
#include "uni_log.h"

#define TCP_CONN_TAG "tcp_conn"

TcpConn :: TcpConn(string &ip, uint16_t port, void *socket, vector<ITcpReceiver *> &receivers, ITcpPacker *packer) {
    mReceiverLock.writeLock();
    mIp = ip;
    mPort = port;
    mSocket = socket;
    mTag = "";
    if (packer) {
        mPacker = packer->copy();
    } else {
        mPacker = NULL;
    }
    vector<ITcpReceiver *> :: iterator it;
    for (it = receivers.begin(); it != receivers.end(); it++) {
        ITcpReceiver *receiver = (*it)->copy();
        receiver->setConn(this);
        mReceivers.push_back(receiver);
    }
    LOGT(TCP_CONN_TAG, "tcp connection created, ip=%s port=%d socket=%p", mIp.c_str(), mPort, mSocket);
    mReceiverLock.writeUnlock();
}

TcpConn :: ~TcpConn() {
    mReceiverLock.writeLock();
    if (mPacker) {
        delete mPacker;
    }
    vector<ITcpReceiver *> :: iterator it;
    for (it = mReceivers.begin(); it != mReceivers.end();) {
        delete *it;
        it = mReceivers.erase(it);
    }
    LOGT(TCP_CONN_TAG, "tcp connection destroyed, ip=%s port=%d", mIp.c_str(), mPort);
    mReceiverLock.writeUnlock();
}

void TcpConn :: setTag(string &tag) {
    mTag = tag;
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
    int pack_len = 0;
    ITcpReceiver *i_receiver = NULL;
    vector<ITcpReceiver *>::iterator it;
    mReceiverLock.readLock();
    if (mPacker) {
        mPacker->packin(data, len);
        while (mPacker->packout(pack_buf, &pack_len) == 0)
        {
            for (vector<ITcpReceiver *>::iterator it = mReceivers.begin(); it != mReceivers.end(); it++)
            {
                i_receiver = *it;
                if (i_receiver->onRecv(pack_buf, pack_len) == 0)
                {
                    data_received_ok = 0;
                    break;
                }
            }
        }
    }
    else
    {
        for (vector<ITcpReceiver *>::iterator it = mReceivers.begin(); it != mReceivers.end(); it++)
        {
            i_receiver = *it;
            if (i_receiver->onRecv(data, len) == 0)
            {
                data_received_ok = 0;
                break;
            }
        }
    }

    mReceiverLock.readUnlock();
    return data_received_ok;
}

//给app调用
int TcpConn :: send(const char *data, int len) {
    LOGT(TCP_CONN_TAG, "send data %s through socket %p", data, mSocket);
    return bufferevent_write((struct bufferevent *)mSocket, data, len);
}
