/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventTcpLink.h
* Summary

* Author: yunmiao
* Date: 2014-6-3 10:36
*******************************************************/

#ifndef EventTcpLink_h__
#define EventTcpLink_h__

#include "EventNet.h"
#include "EventSocket.h"

class EventTcpServer;

class EventTcpLink : public EventSocket
{
public:
	EventTcpLink();
	virtual ~EventTcpLink();

    virtual void        OnClose();
    virtual void        OnSocketWrite(struct bufferevent *pBev);

    void                AttachToServer(EventTcpServer *pSvr);

public:
    int                 GetLinkID() const          { return m_nLinkID; }
    void                SetLinkID(int val)         { m_nLinkID = val; }

    int                 GetClientPort() const      { return m_nClientPort; }
    void                SetClientPort(int val)     { m_nClientPort = val; }

    std::string         GetClientIP() const        { return m_strClientIP; }
    void                SetClientIP(std::string val) { m_strClientIP = val; }

    bool                GetWriteClise() const      { return m_bWriteClose; }
    void                SetWriteClise(bool val)    { m_bWriteClose = val; }

    static int          m_sLinkID;
protected:
	int                 m_nLinkID;
    int                 m_nClientPort;
    
    string              m_strClientIP;
private:
    bool                m_bWriteClose;
    
    // ref
    EventTcpServer      *m_pSvr;
};



#endif // EventTcpLink_h__
