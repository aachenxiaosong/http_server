#include "TcpConnMgr.hpp"
#include <string.h>

TcpConnMgr :: ~TcpConnMgr() {
    mLock.lock();
    mConns.clear();
    mLock.unlock();
}

void TcpConnMgr :: add(const char *ip, uint16_t port, void *socket)
{
    mLock.lock();
    STcpConn *conn = new STcpConn();
    snprintf(conn->ip, sizeof(conn->ip), "%s", ip);
    conn->port = port;
    conn->socket = socket;
    mConns.push_back(conn);
    mLock.unlock();
}

void TcpConnMgr :: del(const char *ip)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (strcmp(conn->ip, ip) == 0) {
            iter = mConns.erase(iter);
        } else {
            ++iter;
        }
    }
    mLock.unlock();
}

void TcpConnMgr :: del(const char *ip, uint16_t port)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn;
    for(iter = mConns.begin(); iter != mConns.end();)
    {
        conn = *iter;
        if (strcmp(conn->ip, ip) == 0 && conn->port == port) {
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

STcpConn *TcpConnMgr :: get(const char *ip, uint16_t port)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (strcmp(conn->ip, ip) == 0 && conn->port == port) {
            break;
        }
    }
    mLock.unlock();
    return conn;
}

STcpConn *TcpConnMgr :: get(const char *ip)
{
    mLock.lock();
    vector<STcpConn *>::iterator iter;
    STcpConn *conn = NULL;
    for(iter = mConns.begin(); iter != mConns.end(); ++iter)
    {
        conn = *iter;
        if (strcmp(conn->ip, ip) == 0) {
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