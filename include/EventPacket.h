/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventPacket.h
* Summary

* Author: yunmiao
* Date: 2014-5-27 15:46
*******************************************************/

#ifndef EventPacket_h__
#define EventPacket_h__

#include "NET_MSG_TYPE.h"
#include <string.h>
#include <stdlib.h>

#define PACKET_HEAD     (0x55AA)

#ifdef WIN32
#pragma warning (disable: 4200)
#endif

#pragma pack(push,1)
typedef struct _packet_base_st
{
    short       m_i2Begin;
    short       m_i2PacketID;
    short       m_i2DataSize;
    // 玩家GUID
    char        m_szGuid[32];

    char        m_szData[0];
    _packet_base_st() 
    {
        memset(this, 0, sizeof(*this));
    }
}EventNetPacket;
#pragma pack(pop)

class EventSocket;

class EventNetMsg
{
public:
    EventNetMsg();
    ~EventNetMsg();

    static void sInit();
    static int *m_sMsgSize;

    // 定长消息包初始化
    bool InitFromMsgType(short nPackID);
    // 可变消息包初始化
    bool InitFromMsgType0(short nPackID, int nLen);

    void SendTo(EventSocket *pSocket);

    EventNetPacket  *GetHeader()        { return (EventNetPacket *)m_pPacketBuff; }
    char            *GetBody()          { return m_pPacketBuff + sizeof(EventNetPacket); }
private:
    int             m_nPacketLen;
    char            *m_pPacketBuff;
};


#endif // EventPacket_h__