#include "TcpConnMgr.hpp"
#include <string.h>

TcpConnMgr :: ~TcpConnMgr() {
    mLock.lock();
    mConns.clear();
    mLock.unlock();
}

void TcpConnMgr :: add(string ip, uint16_t port, void *socket)
{
    mLock.lock();
    STcpConn *conn = new STcpConn();
    conn->ip = ip;
    conn->port = port;
    conn->socket = socket;
    conn->tag = "";
    mConns.push_back(conn);
    mLock.unlock();
}

void TcpConnMgr :: del(string ip)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (conn->ip.compare(ip) == 0) {
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
    vector<STcpConn *>::iterator iter;
    STcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (conn->ip.compare(ip) == 0 && conn->port == port) {
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
    vector<STcpConn *>::iterator iter;
    STcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (conn->socket == socket) {
            iter = mConns.erase(iter);
        } else {
            ++iter;
        }
    }
    mLock.unlock();
}

STcpConn *TcpConnMgr :: get(string ip, uint16_t port)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->ip.compare(ip) == 0 && conn->port == port) {
            break;
        }
    }
    mLock.unlock();
    return conn;
}

STcpConn *TcpConnMgr :: get(string ip)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->ip.compare(ip) == 0) {
            break;
        }
    }
    mLock.unlock();
    return conn;
}

STcpConn *TcpConnMgr :: getByTag(string tag)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->tag.compare(tag) == 0) {
            break;
        }
    }
    mLock.unlock();
    return conn;
}

STcpConn *TcpConnMgr :: get(void *socket)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (conn->socket == socket) {
            break;
        }
    }
    mLock.unlock();
    return conn;
}

int TcpConnMgr :: setTag(STcpConn *conn, string tag) {
    bool conn_exist = false;
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        if (*iter == conn) {
            conn_exist = true;
            conn->tag = tag;
            break;
        }
    }
    mLock.unlock();
    return (conn_exist == true);
}
