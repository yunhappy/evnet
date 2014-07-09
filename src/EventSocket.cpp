/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventSocket.cpp
* Summary

* Author: yunmiao
* Date: 2014-5-23 11:41
*******************************************************/

#include "EventSocket.h"
#include "CommonMacros.h"
#include "EventPacket.h"
#include "EventParser.h"
#include "EventMain.h"
#include "CommonProfiler.h"

EventSocket::EventSocket()
{
    m_nSocket = 0;

    m_pBuffev = NULL;
    m_pParser = new EventParser(this);

    m_bTick = false;

    m_fnOnClose = NULL;
    m_pOnCloseData = NULL;
}

EventSocket::~EventSocket()
{
    if (m_pBuffev != NULL)
    {
        bufferevent_free(m_pBuffev);
        m_pBuffev = NULL;
    }

    SAFE_DELETE(m_pParser);
}

bool EventSocket::InitTick( int sec, int ms )
{
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = ms * 1000;

    event_assign(&m_tickEv, g_pEvNetBase, -1, EV_PERSIST, &EventSocket::sOnTick, (void *)this);

    event_add(&m_tickEv, &tv);

    m_bTick = true;

    return true;
}

bool EventSocket::Create( evutil_socket_t nFd /*= 0*/ )
{
    if (nFd == 0)
    {
        m_nSocket = -1;
    }
    else
    {
        m_nSocket = nFd;
    }

#ifdef EVNET_USE_THREAD
    m_pBuffev = bufferevent_socket_new(g_pEvNetBase, m_nSocket, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
#else
    m_pBuffev = bufferevent_socket_new(g_pEvNetBase, m_nSocket, BEV_OPT_CLOSE_ON_FREE);
#endif

    if (m_pBuffev == NULL)
    {
        int err = EVUTIL_SOCKET_ERROR();
        printf("%d %s\n", err, evutil_socket_error_to_string(err));
        return false;
    }

    if (nFd == 0)
    {
        m_nSocket = bufferevent_getfd(m_pBuffev);
    }

    bufferevent_setcb(m_pBuffev, &EventSocket::sOnRead, &EventSocket::sOnWrite, &EventSocket::sOnEvent, this);
    bufferevent_enable(m_pBuffev, EV_READ | EV_WRITE);

    bool bOptVal = true;
    int nOptLen = sizeof(bool);
    setsockopt(m_nSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&bOptVal, nOptLen);

    int nOptVal = 1024 * 1024;
    nOptLen = sizeof(int);
    setsockopt(m_nSocket, SOL_SOCKET, SO_SNDBUF, (const char *)&nOptVal, nOptLen);
    setsockopt(m_nSocket, SOL_SOCKET, SO_RCVBUF, (const char *)&nOptVal, nOptLen);

    return true;
}


void EventSocket::Send( const char *buf, const int nLen )
{
    bufferevent_write(m_pBuffev, (void *)buf, nLen);
}


int EventSocket::SendNetMsg(EventNetPacket *pMsg)
{
    if (pMsg == NULL)
    {
        return 0;
    }

    common_profiler::m_nPacketSendCount ++;
    common_profiler::m_nSendBytes += pMsg->m_i2DataSize + sizeof(EventNetPacket);

    Send((char *)pMsg, pMsg->m_i2DataSize + sizeof(EventNetPacket));
    return pMsg->m_i2DataSize + sizeof(EventNetPacket);
}

void EventSocket::OnSocketRead()
{
    bufferevent_read_buffer(m_pBuffev, m_pParser->GetCache());
    int ret = 0;
    do 
    {
        ret = m_pParser->Parse();
    } while (ret != PARSER_RET_NEED_MORE);

    return;
}

void EventSocket::OnSocketEvent( short events )
{
    if (events & BEV_EVENT_EOF)
    {
        printf("[%p] fd[%d] connection close\n", this, m_nSocket);
        OnClose();
    }
    else if (events & BEV_EVENT_ERROR)
    {
        int err = EVUTIL_SOCKET_ERROR();
        printf("[%p] fd[%d] Got an error %d (%s) close\n", 
            this, m_nSocket, err, evutil_socket_error_to_string(err));
        OnClose();
    }
    else if (events & BEV_EVENT_CONNECTED)
    {
        printf("[%p] fd[%d] connected\n", this, m_nSocket);
        OnConnect();
    }
    
}

void EventSocket::OnTick()
{
    // do nothing
}

void EventSocket::OnClose()
{
    if (m_pBuffev == NULL)
    {
        return;
    }

    bufferevent_free(m_pBuffev);
    m_pBuffev = NULL;
    m_nSocket = 0;

    if (m_fnOnClose != NULL)
    {
        (*m_fnOnClose)(this, m_pOnCloseData);
    }
}

void EventSocket::OnConnect()
{
    GetParser()->SetCallBack(sOnNewMsg, this);

    m_nSocket = bufferevent_getfd(m_pBuffev);

    bool bOptVal = true;
    int nOptLen = sizeof(bool);
    setsockopt(m_nSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&bOptVal, nOptLen);

    int nOptVal = 1024 * 1024;
    nOptLen = sizeof(int);
    setsockopt(m_nSocket, SOL_SOCKET, SO_SNDBUF, (const char *)&nOptVal, nOptLen);
    setsockopt(m_nSocket, SOL_SOCKET, SO_RCVBUF, (const char *)&nOptVal, nOptLen);
}

void EventSocket::OnNewMsg(EventSocket *pSocket, EventNetPacket *pMsg)
{
    printf("new msg\n");
}

void EventSocket::sOnTick( evutil_socket_t fd, short event, void *pData )
{
    EventSocket *pSocket = (EventSocket *)pData;
    if (pSocket == NULL)
    {
        return;
    }

    pSocket->OnTick();
}

void EventSocket::sOnRead( struct bufferevent *pBev, void *pData )
{
    EventSocket *pSocket = (EventSocket *)pData;
    if (pSocket == NULL)
    {
        return;
    }

    //TODO
    // main thread read
    pSocket->OnSocketRead();
}

// do nothing
void EventSocket::sOnWrite( struct bufferevent *pBev, void *pData )
{
    EventSocket *pSocket = (EventSocket *)pData;
    if (pSocket == NULL)
    {
        return;
    }

    pSocket->OnSocketWrite(pBev);
}

void EventSocket::sOnEvent( struct bufferevent *pBev, short events, void *pData )
{
    EventSocket *pSocket = (EventSocket *)pData;
    if (pSocket == NULL)
    {
        return;
    }

    //TODO
    pSocket->OnSocketEvent(events);
}

//TODO
void EventSocket::sOnNewMsg(EventSocket *pSocket, EventNetPacket *pMsg, void *pData)
{
    EventSocket *pBaseSocket = (EventSocket *)pData;
    if (pBaseSocket == NULL)
    {
        return;
    }

    //pBaseSocket->OnNewMsg(pSocket, pMsg);

    common_profiler::m_nPacketRecvCount ++;
    common_profiler::m_nRecvBytes += pMsg->m_i2DataSize + sizeof(EventNetPacket);

    // 放入消息队列
    MainThreadData *pTData = new MainThreadData;
    pTData->m_pSocket = pSocket;
    //pTData->m_pMsg = pMsg;
    pTData->m_pMsg = (EventNetPacket *)malloc(pMsg->m_i2DataSize + sizeof(EventNetPacket));
    memcpy(pTData->m_pMsg, pMsg, (pMsg->m_i2DataSize + sizeof(EventNetPacket)));

    g_pEventMain->PushData(pTData);
}



bool socket_keep_alive( int nSocket )
{
    int nKeepAlive = 1; // 开启keepalive属性
    int nKeepIdle = 60; // 如果该连接在60秒内无数据往来，则进行探测
    int nKeepInterval = 5;  // 探测发包间隔为5秒
    int nKeepCount = 3;     // 尝试探测的次数。如果第一次探测包就收到响应，则不再探测了

#ifndef WIN32
    if (setsockopt(nSocket, SOL_SOCKET, SO_KEEPALIVE, (void *)&nKeepAlive, sizeof(nKeepAlive)) != 0)
    {
        return false;
    }

    if (setsockopt(nSocket, SOL_TCP, TCP_KEEPIDLE, (void *)&nKeepIdle, sizeof(nKeepIdle)) != 0)
    {
        return false;
    }

    if (setsockopt(nSocket, SOL_TCP, TCP_KEEPINTVL, (void *)&nKeepInterval, sizeof(nKeepInterval)) != 0)
    {
        return false;
    }

    if (setsockopt(nSocket, SOL_TCP, TCP_KEEPCNT, (void *)&nKeepCount, sizeof(nKeepCount)) != 0)
    {
        return false;
    }
#endif

    return true;
}
