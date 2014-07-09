/*******************************************************
* Copyright (c) 2014, ooxxstudio All rights reserved.
* 
* FileName: CommonThread.cpp
* Summary

* Author: yunmiao
* Date: 2014-6-20 10:58
*******************************************************/

#include "CommonThread.h"


CommonThread::CommonThread()
{
    m_status = READY;

#ifdef _WIN32
    m_nTID = 0;
    m_hThread = 0;
#else
    m_nTID = 0;
#endif
}

CommonThread::~CommonThread()
{
    m_status = READY;

#ifdef _WIN32
    m_nTID = 0;
    m_hThread = 0;
#else
    m_nTID = 0;
#endif
}

void CommonThread::Start()
{
    if (m_status != READY)
    {
        return;
    }

#ifdef _WIN32
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, sThreadProc, this, 0, &m_nTID);
#else
    pthread_create(&m_nTID, NULL, sThreadProc, this);
#endif  

    return;
}

void CommonThread::Stop()
{

}

void CommonThread::Run()
{

}

void CommonThread::Exit()
{

}

void CommonThread::PushData(void *val)
{
    if (val == NULL)
    {
        return;
    }

    m_asynQueue.push(val);
}

bool CommonThread::TryPop(void *&val)
{
    return m_asynQueue.try_pop(val);
}

#ifdef _WIN32
unsigned __stdcall CommonThread::sThreadProc( void *param )
{
    CommonThread *pThread = (CommonThread *)param;
    if (pThread == NULL)
    {
        return 0;
    }

    pThread->SetStatus(CommonThread::RUNNING);

    pThread->Run();

    pThread->SetStatus(CommonThread::EXIT);

    pThread->Exit();

    _endthreadex(0);

    return 1;
}
#else
void * CommonThread::sThreadProc( void *param )
{
    CommonThread *pThread = (CommonThread *)param;
    if (pThread == NULL)
    {
        return NULL;
    }

    pThread->SetStatus(CommonThread::RUNNING);

    pThread->Run();

    pThread->SetStatus(CommonThread::EXIT);

    pThread->Exit();

    return NULL;
}



#endif
