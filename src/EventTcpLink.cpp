/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventTcpLink.cpp
* Summary

* Author: yunmiao
* Date: 2014-6-3 10:46
*******************************************************/

#include "EventTcpLink.h"
#include "EventTcpServer.h"

int EventTcpLink::m_sLinkID = 0;


EventTcpLink::EventTcpLink()
{
    m_nLinkID = 0;
    m_nClientPort = 0;
    m_nLinkID = ++m_sLinkID;

    m_bWriteClose = false;
    m_pSvr = NULL;
}


EventTcpLink::~EventTcpLink()
{

}

void EventTcpLink::OnClose()
{
    printf("[%p]\n", this);

    m_pSvr->OnLinkClose(this);
}

void EventTcpLink::OnSocketWrite( struct bufferevent *pBev )
{
    if (m_bWriteClose)
    {
        OnClose();
    }
}

void EventTcpLink::AttachToServer( EventTcpServer *pSvr )
{
    m_pSvr = pSvr;
}
