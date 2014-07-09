/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventSocket.h
* Summary

* Author: yunmiao
* Date: 2014-5-23 11:12
*******************************************************/

#ifndef EventSocket_h__
#define EventSocket_h__

#include "EventNet.h"
#include "EventPacket.h"

class EventParser;

class EventSocket
{
public:
    typedef void (* fpOnCloseCB)(EventSocket *pSocket, void *pData);
public:
    EventSocket();
    virtual ~EventSocket();

    // ≥ı ºªØTICK
    bool            InitTick(int sec, int ms);
    bool            Create(evutil_socket_t nFd = 0);

    void            Send(const char *buf, const int nLen);
    virtual int     SendNetMsg(EventNetPacket *pMsg);

    virtual void    OnSocketRead();
    virtual void    OnSocketWrite(struct bufferevent *pBev) {}
    virtual void    OnSocketEvent(short events);

    virtual void    OnTick();
    virtual void    OnClose();
    virtual void    OnConnect();

    virtual void    OnNewMsg(EventSocket *pSocket, EventNetPacket *pMsg);

    static void     sOnTick(evutil_socket_t fd, short event, void *pData);
    static void     sOnRead(struct bufferevent *pBev, void *pData);
    static void     sOnWrite(struct bufferevent *pBev, void *pData);
    static void     sOnEvent(struct bufferevent *pBev, short events, void *pData);
    static void     sOnNewMsg(EventSocket *pSocket, EventNetPacket *pMsg, void *pData);


    EventParser     *GetParser() const            { return m_pParser; }

protected:
    evutil_socket_t     m_nSocket;

    struct bufferevent  *m_pBuffev;
    EventParser         *m_pParser;

private:
    bool            m_bTick;
    struct event    m_tickEv;


    fpOnCloseCB     m_fnOnClose;
    void            *m_pOnCloseData;
};

bool socket_keep_alive(evutil_socket_t nSocket);

#endif // EventSocket_h__
