/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventTcpServer.cpp
* Summary

* Author: yunmiao
* Date: 2014-6-3 9:51
*******************************************************/

#include "EventTcpServer.h"
#include "CommonMacros.h"
#include "EventTcpLink.h"
#include "EventParser.h"
#include "CommonProfiler.h"

EventTcpServer::EventTcpServer()
{
    m_nPort = 0;
    m_pListener = NULL;
    m_PacketStat.Clear();
    m_bStat = false;
}

EventTcpServer::~EventTcpServer()
{
    m_nPort = 0;

    if (m_pListener != NULL)
    {
        evconnlistener_free(m_pListener);
        m_pListener = NULL;
    }
    m_PacketStat.Clear();
    m_bStat = false;
}

bool EventTcpServer::Listen( unsigned int nPort )
{
    if (nPort == 0)
    {
        //TODO log
        return false;
    }

    m_nPort = nPort;

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(m_nPort);

    m_pListener = evconnlistener_new_bind(g_pEvNetBase, &sOnAccept, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE | LEV_OPT_THREADSAFE,
        -1, (struct sockaddr*)&sin, sizeof(sin));
    if (m_pListener == NULL)
    {
        //TODO log
        return false;
    }

    printf("Listen On %d success\n", m_nPort);

    m_nSocket = evconnlistener_get_fd(m_pListener);

    evconnlistener_set_error_cb(m_pListener, &sOnAcceptError);

    return true;
}

void EventTcpServer::OnLinkClose(EventTcpLink *pLink)
{
    m_mapLink.erase(pLink);
    SAFE_DELETE(pLink);
}

EventSocket * EventTcpServer::OnAccept( evutil_socket_t fd, struct sockaddr *address, int socklen )
{
    char szIP[64] = {0};
    int nPort = ntohs( ((struct sockaddr_in *)address)->sin_port );
    strncpy(szIP, inet_ntoa( ((struct sockaddr_in *)address)->sin_addr ), sizeof(szIP));

    EventTcpLink *pLink = new EventTcpLink;

    printf("[%p] fd[%d] ip[%s:%d]\n", pLink, fd, szIP, nPort);

    if (!pLink->Create(fd))
    {
        return NULL;
    }

    pLink->AttachToServer(this);
    pLink->SetClientIP(szIP);
    pLink->SetClientPort(nPort);

    pLink->GetParser()->SetCallBack(sOnNewMsg, this);

    m_mapLink[pLink] = pLink;

    return pLink;
}

void EventTcpServer::sOnAccept( struct evconnlistener *pListener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *pCtx )
{
    EventTcpServer *pServer = (EventTcpServer *)pCtx;
    if (pServer == NULL)
    {
        return;
    }

    pServer->OnAccept(fd, address, socklen);
}

void EventTcpServer::sOnAcceptError( struct evconnlistener *pListener, void *pCtx )
{
    struct event_base *pBase = evconnlistener_get_base(pListener);
    int err = EVUTIL_SOCKET_ERROR();
    printf("get an error %d (%s) on the listener\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(pBase, NULL);
}

int EventTcpServer::SendNetMsg( EventNetPacket *pMsg )
{
    if (m_bStat)
    {
        m_PacketStat.m_nSendNum ++;
        m_PacketStat.m_nSendByte += pMsg->m_i2DataSize + sizeof(EventNetPacket);
        m_PacketStat.m_MsgIDArr[pMsg->m_i2PacketID] ++;
    } 

    return EventSocket::SendNetMsg(pMsg);
}

void EventTcpServer::OnNewMsg( EventSocket *pSocket, EventNetPacket *pMsg )
{
//     common_profiler::m_nPacketRecvCount ++;
//     common_profiler::m_nRecvBytes += pMsg->m_i2DataSize + sizeof(EventNetPacket);

    if (m_bStat)
    {
        m_PacketStat.m_nRecvNum ++;
        m_PacketStat.m_nRecvByte += pMsg->m_i2DataSize + sizeof(EventNetPacket);
        m_PacketStat.m_MsgIDArr[pMsg->m_i2PacketID] ++;
    }
}

void EventTcpServer::OnTick()
{
    //to-do ≈–∂œ ±º‰º‰∏Ù;
    if (m_bStat)
    {
        m_PacketStat.Print(false);
        m_PacketStat.Clear();
        m_PacketStat.Print(false);
    }
}

