/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventTcpClient.cpp
* Summary

* Author: yunmiao
* Date: 2014-6-3 15:20
*******************************************************/

#include "EventTcpClient.h"
#include "EventParser.h"


EventTcpClient::EventTcpClient()
{
    m_nDestPort = 0;
}

EventTcpClient::~EventTcpClient()
{

}

bool EventTcpClient::Connect( const char *szDest, unsigned int nPort )
{
    m_strDestIP = szDest;
    m_nDestPort = nPort;

    Create();

    if (bufferevent_socket_connect_hostname(m_pBuffev, NULL, AF_INET, m_strDestIP.c_str(), m_nDestPort) == -1)
    {
        printf("connect operation error\n");
        return false;
    }

    printf("connect operation %p [%s:%d]\n", this, szDest, nPort);

    return true;
}


void EventTcpClient::OnTick()
{
    // ¶ÏÏßÖØÁ¬
    if (m_nSocket == 0)
    {
        Connect(m_strDestIP.c_str(), m_nDestPort);
    }
}

void EventTcpClient::OnConnect()
{
    printf("connect success %p [%s:%d]\n", this, m_strDestIP.c_str(), m_nDestPort);

    m_nSocket = bufferevent_getfd(m_pBuffev);

    bool bOptVal = true;
    int nOptLen = sizeof(bool);
    setsockopt(m_nSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&bOptVal, nOptLen);

    int nOptVal = 1024 * 1024;
    nOptLen = sizeof(int);
    setsockopt(m_nSocket, SOL_SOCKET, SO_SNDBUF, (const char *)&nOptVal, nOptLen);
    setsockopt(m_nSocket, SOL_SOCKET, SO_RCVBUF, (const char *)&nOptVal, nOptLen);

    socket_keep_alive(m_nSocket);

    GetParser()->SetCallBack(sOnNewMsg, this);

    HandShake();
}

void EventTcpClient::HandShake()
{

}
