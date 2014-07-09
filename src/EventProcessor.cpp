/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventProcessor.cpp
* Summary: 

* Author: yunmiao
* Date: 2014/06/29  10:27
*******************************************************/

#include "EventProcessor.h"
#include <string.h>

EventProcessor::EventProcessor()
{
    m_pSubject = NULL;
    memset(m_mapProcessor, 0, sizeof(m_mapProcessor));
}

EventProcessor::EventProcessor(void *pSubject) :m_pSubject(pSubject)
{
    memset(m_mapProcessor, 0, sizeof(m_mapProcessor));
}

void EventProcessor::OnNewMsg(NET_MSG_TYPE type, void *pData)
{
    if (type > MSG_TYPE_MIN && type < MSG_TYPE_MAX)
    {
        fpProcessFunc pFunc = m_mapProcessor[type];
        if (pFunc)
        {
            //(this->*pFunc)(m_pSubject , pData);
            (*pFunc)(m_pSubject , pData);
        }
        else
        {
            //TODO
        }
    }
    
}

void EventProcessor::ProcessRegister(NET_MSG_TYPE type, fpProcessFunc fpFunc)
{
    if (type > MSG_TYPE_MIN && type < MSG_TYPE_MAX)
    {
        m_mapProcessor[type] = fpFunc;
    }
}

