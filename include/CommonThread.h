/*******************************************************
* Copyright (c) 2014, ooxxstudio All rights reserved.
* 
* FileName: CommonThread.h
* Summary

* Author: yunmiao
* Date: 2014-6-20 10:31
*******************************************************/

#ifndef Thread_h__
#define Thread_h__

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

#include <tbb/concurrent_queue.h>

class CommonThread
{
public:
    enum ThreadStatus
    {
        READY,
        RUNNING,
        EXITING,
        EXIT,
    }; 
public:
    CommonThread();
    virtual ~CommonThread();

    void            Start();
    virtual void    Stop();
    virtual void    Run();

    void            Exit();

    CommonThread::ThreadStatus   GetStatus() const                       { return m_status; }
    void                    SetStatus(CommonThread::ThreadStatus val)    { m_status = val; }

    void            PushData(void *val);
    bool            TryPop(void *&val);
protected:
    tbb::concurrent_queue<void *>   m_asynQueue;

private:
#ifdef _WIN32
    static unsigned __stdcall sThreadProc(void *param);
#else
    static void *sThreadProc(void *param);
#endif

private:
    ThreadStatus    m_status;
    
#ifdef _WIN32
    unsigned int    m_nTID;
    HANDLE          m_hThread;
#else
    pthread_t       m_nTID;
#endif
};

#endif // Thread_h__