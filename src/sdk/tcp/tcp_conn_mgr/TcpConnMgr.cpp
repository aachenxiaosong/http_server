#include "TcpConnMgr.hpp"
#include <string.h>

void TcpConnMgr ::add(char *ip, uint16_t port, void *socket)
{
    mLock.lock();
    STcpConn *conn = new STcpConn();
    snprintf(conn->ip, sizeof(conn->ip), "%s", ip);
    conn->port = port;
    conn->socket = socket;
    mConns.push_back(conn);
    mLock.unlock();
}

void TcpConnMgr ::del(char *ip)
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

void TcpConnMgr ::del(char *ip, uint16_t port)
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

void TcpConnMgr ::del(void *socket)
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

void *TcpConnMgr ::get(char *ip, uint16_t port)
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

void *TcpConnMgr ::get(char *ip)
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