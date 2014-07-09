/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventTcpServer.h
* Summary

* Author: yunmiao
* Date: 2014-6-3 9:42
*******************************************************/

#ifndef EventTcpServer_h__
#define EventTcpServer_h__

#include "EventNet.h"
#include "EventSocket.h"
#include "CommonStruct.h"
#include <string>

using namespace std;

class EventTcpLink;
class EventParser;

class EventTcpServer : public EventSocket
{
public:
    typedef map <EventTcpLink *, EventTcpLink *> TCPLINK_MAP;

public:
	EventTcpServer();
	virtual ~EventTcpServer();

    bool            Listen(unsigned int nPort);

    virtual void    OnLinkClose(EventTcpLink *pLink);
    virtual EventSocket *OnAccept(evutil_socket_t fd, struct sockaddr *address, int socklen);

    static void     sOnAccept(struct evconnlistener *pListener, evutil_socket_t fd, 
                                    struct sockaddr *address, int socklen, void *pCtx);
    static void     sOnAcceptError(struct evconnlistener *pListener, void *pCtx);

    virtual int     SendNetMsg(EventNetPacket *pMsg);
    virtual void    OnNewMsg(EventSocket *pSocket, EventNetPacket *pMsg);

    virtual void    OnTick();

public:
    unsigned int    GetPort() const             { return m_nPort; }
    void            SetPort(unsigned int val)   { m_nPort = val; }

    std::string     GetIP() const               { return m_szIP; }
    void            SetIP(std::string val)      { m_szIP = val; }

    void            SetPacketStat(bool val)     { m_bStat = val; }
protected:
    unsigned int    m_nPort;
    string          m_szIP;
    
	evconnlistener  *m_pListener;

    TCPLINK_MAP     m_mapLink;
    // 网络包统计;
    PacketStat      m_PacketStat;
private:
    // 是否统计;
    bool            m_bStat;
};

#endif // EventTcpServer_h__