#include "TcpHandle.hpp"
#include "event2/bufferevent.h"
#include <string.h>


int TcpHandle ::setPacker(IProtocolPacker *packer)
{
    mPacker = packer;
    return 0;
}

int TcpHandle ::addReceiver(ITcpReceiver *receiver)
{
    mReceiverLock.writeLock();
    mReceivers.push_back(receiver);
    mReceiverLock.writeUnlock();
    return 0;
}

void TcpHandle ::delReceiver(ITcpReceiver *receiver)
{
    vector<ITcpReceiver *>::iterator it;
    ITcpReceiver *i_receiver = NULL;
    mReceiverLock.writeLock();
    for (it = mReceivers.begin(); it != mReceivers.end();) {
        i_receiver = *it;
        if (i_receiver == receiver) {
            it = mReceivers.erase(it);
        } else {
            it++;
        }
    }
    mReceiverLock.writeUnlock();
}

int TcpHandle ::onRecv(STcpConn *conn, const char *data, int len)
{
    int data_received_ok = -1;
    char pack_buf[MAX_TCP_PACK_LEN] = {0};
    int pack_len = 0;
    ITcpReceiver *i_receiver = NULL;
    vector<ITcpReceiver *>::iterator it;
    if (mPacker) {
        if (mPacker->pack(data, len, pack_buf, &pack_len) != 0) {
            //不够一个帧,防止粘包和半包
            return 0;
        }
    } else {
        memcpy(pack_buf, data, len);
        pack_len = len;
    }
    mReceiverLock.readLock();
    for (vector<ITcpReceiver *>::iterator it = mReceivers.begin(); it != mReceivers.end(); it++) {
        i_receiver = *it;
        if (i_receiver->onRecv(conn, pack_buf, pack_len) == 0) {
            data_received_ok = 0;
            break;
        }
    }
    mReceiverLock.readUnlock();
    return data_received_ok;
}

int TcpHandle ::send(STcpConn *conn, const char *data, int len)
{
    return bufferevent_write((struct bufferevent *)conn->socket, data, len);
}