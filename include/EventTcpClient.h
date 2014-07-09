/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventTcpClient.h
* Summary

* Author: yunmiao
* Date: 2014-6-3 15:16
*******************************************************/

#ifndef EventTcpClient_h__
#define EventTcpClient_h__

#include "EventNet.h"
#include "EventSocket.h"

class EventTcpLink;

class EventTcpClient : public EventSocket
{
public:
	EventTcpClient();
	virtual ~EventTcpClient();

    bool            Connect(const char *szDest, unsigned int nPort);

    virtual void    OnTick();
    virtual void    OnConnect();

    virtual void    HandShake();

protected:
	
    int             m_nDestPort;
    string          m_strDestIP;
private:

};
#endif // EventTcpClient_h__