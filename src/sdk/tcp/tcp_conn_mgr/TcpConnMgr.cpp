#include "TcpConnMgr.hpp"
#include <string.h>

TcpConnMgr :: ~TcpConnMgr() {
    mLock.lock();
    mConns.clear();
    mLock.unlock();
}

void TcpConnMgr :: add(string ip, uint16_t port, void *socket, vector<ITcpReceiver *> &receivers, ITcpPacker *packer)
{
    mLock.lock();
    TcpConn *conn = new TcpConn(ip, port, socket, receivers, packer);
    mConns.push_back(conn);
    mLock.unlock();
}

void TcpConnMgr :: del(string ip)
{
    mLock.lock();
    vector<TcpConn *>::iterator iter;
    TcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (conn->getIp().compare(ip) == 0) {
            iter = mConns.erase(iter);
        } else {
            ++iter;
        }
    }
    mLock.unlock();
}

void TcpConnMgr :: del(string ip, uint16_t port)
{
    mLock.lock();
    vector<TcpConn *>::iterator iter;
    TcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (conn->getIp().compare(ip) == 0 && conn->getPort() == port) {
            iter = mConns.erase(iter);
        } else {
            ++iter;
        }
    }
    mLock.unlock();
}

void TcpConnMgr :: del(void *socket)
{
    mLock.lock();
    vector<TcpConn *>::iterator iter;
    TcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (conn->getSocket() == socket) {
            iter = mConns.erase(iter);
        } else {
            ++iter;
        }
    }
    mLock.unlock();
}

TcpConn *TcpConnMgr :: get(string ip, uint16_t port)
{
    mLock.lock();
    vector<TcpConn *>::iterator iter;
    TcpConn *ret = NULL;
    TcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->getIp().compare(ip) == 0 && conn->getPort() == port) {
            ret = conn;
            break;
        }
    }
    mLock.unlock();
    return ret;
}

TcpConn *TcpConnMgr :: get(string ip)
{
    mLock.lock();
    vector<TcpConn *>::iterator iter;
    TcpConn *ret = NULL;
    TcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->getIp().compare(ip) == 0) {
            ret = conn;
            break;
        }
    }
    mLock.unlock();
    return ret;
}

TcpConn *TcpConnMgr :: getByTag(string tag)
{
    mLock.lock();
    vector<TcpConn *>::iterator iter;
    TcpConn *ret = NULL;
    TcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->getTag().compare(tag) == 0) {
            ret = conn;
            break;
        }
    }
    mLock.unlock();
    return ret;
}

TcpConn *TcpConnMgr :: get(void *socket)
{
    mLock.lock();
    vector<TcpConn *>::iterator iter;
    TcpConn *ret = NULL;
    TcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->getSocket() == socket) {
            ret = conn;
            break;
        }
    }
    mLock.unlock();
    return ret;
}
