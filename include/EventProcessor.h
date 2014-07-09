/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventProcessor.h
* Summary: 

* Author: yunmiao
* Date: 2014/06/29  10:26
*******************************************************/

#ifndef EventProcessor_h__
#define EventProcessor_h__


#include "NET_MSG_TYPE.h"

typedef void (*fpProcessFunc)(void *pSubject, void *pData);

class EventProcessor
{   
public:
    EventProcessor();
	EventProcessor(void *pSubject);
	virtual ~EventProcessor() {}
	
    void OnNewMsg(NET_MSG_TYPE type, void *pData);
    void ProcessRegister(NET_MSG_TYPE type, fpProcessFunc fpFunc);
private:
    fpProcessFunc   m_mapProcessor[NET_MSG_TYPE_MAX];
    void            *m_pSubject;
};


#endif // EventProcessor_h__
