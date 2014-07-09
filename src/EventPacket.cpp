/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventPacket.cpp
* Summary: 

* Author: yunmiao
* Date: 2014/06/15  15:38
*******************************************************/

#include "EventPacket.h"
#include "EventSocket.h"
#include "NET_MSG_TYPE.h"
#include "PACKET_STRUCT.h"
#include "EventMain.h"

int * EventNetMsg::m_sMsgSize = NULL;

EventNetMsg::EventNetMsg()
{
    m_nPacketLen = 0;
    m_pPacketBuff = NULL;
}

EventNetMsg::~EventNetMsg()
{
    if (m_pPacketBuff != NULL)
    {
        free(m_pPacketBuff);
    }
}

bool EventNetMsg::InitFromMsgType(short nPackID)
{
    if (nPackID >= MSG_TYPE_MAX || m_sMsgSize == NULL)
    {
        return false;
    }

    int nBodySize = m_sMsgSize[nPackID];
    if (nBodySize <= 0) // 可变长度
    {
        return false;
    }
   
    if (m_pPacketBuff != NULL)
    {
        free(m_pPacketBuff);
    }

    m_pPacketBuff = (char *)calloc(1, nBodySize + sizeof(EventNetPacket));
    EventNetPacket *pHeader = GetHeader();

    pHeader->m_i2PacketID = nPackID;
    pHeader->m_i2Begin = PACKET_HEAD;
    pHeader->m_i2DataSize = nBodySize;
    memset(pHeader->m_szGuid, 0, sizeof(pHeader->m_szGuid));

    m_nPacketLen = sizeof(EventNetPacket) + nBodySize;


    return true;
}

bool EventNetMsg::InitFromMsgType0(short nPackID, int nBodyLen)
{
    if (nPackID >= MSG_TYPE_MAX || m_sMsgSize == NULL)
    {
        return false;
    }

    int nSize = m_sMsgSize[nPackID];
    if (nSize != 0) // 可变长度
    {
        return false;
    }

    if (m_pPacketBuff != NULL)
    {
        free(m_pPacketBuff);
    }

    m_pPacketBuff = (char *)calloc(1, nBodyLen + sizeof(EventNetPacket));
    EventNetPacket *pHeader = GetHeader();

    pHeader->m_i2PacketID = nPackID;
    pHeader->m_i2Begin = PACKET_HEAD;
    pHeader->m_i2DataSize = nBodyLen;

    m_nPacketLen = sizeof(EventNetPacket) + nBodyLen;

    return true;
}

void EventNetMsg::sInit()
{
    if (m_sMsgSize == NULL)
    {
        m_sMsgSize = new int[MSG_TYPE_MAX];
    }

    m_sMsgSize[MSG_TYPE_MIN] = 0;
    m_sMsgSize[CL_MSG_TYPE_MIN] = 0;
    m_sMsgSize[CW_MSG_TYPE_MIN] = 0;
    m_sMsgSize[CS_MSG_TYPE_MIN] = 0;
    m_sMsgSize[SC_MSG_MIN] = 0;

#define _NET_MSG_DEF(type, n)  m_sMsgSize[type] = sizeof(_##type);
#include "INNER_NET_MSG.h"

#define _NET_MSG_DEF(type, n)  m_sMsgSize[type] = sizeof(_##type);
#include "CL_NET_MSG.h"

#define _NET_MSG_DEF(type, n)  m_sMsgSize[type] = sizeof(_##type);
#include "SC_NET_MSG.h"

#define _NET_MSG_DEF(type, n)  m_sMsgSize[type] = sizeof(_##type);
#include "CW_NET_MSG.h"

#define _NET_MSG_DEF(type, n)  m_sMsgSize[type] = sizeof(_##type);
#include "CS_NET_MSG.h"


    // 可变长度
    m_sMsgSize[ROBOT_TEST_PACKET] = 0;

}

void EventNetMsg::SendTo(EventSocket *pSocket)
{
    if (pSocket == NULL)
    {
        return;
    }

    //pSocket->Send(m_pPacketBuff, m_nPacketLen);
    //pSocket->SendNetMsg(GetHeader());

    // 放入消息队列
    MainThreadData *pTData = new MainThreadData;
    pTData->m_pSocket = pSocket;
    //pTData->m_pMsg = GetHeader();
    pTData->m_pMsg = (EventNetPacket *)malloc(m_nPacketLen);
    memcpy(pTData->m_pMsg, GetHeader(), m_nPacketLen);
    g_pEventNet->PushData(pTData);
}


