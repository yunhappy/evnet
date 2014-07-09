/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventMainApp.h
* Summary

* Author: yunmiao
* Date: 2014-6-9 15:53
*******************************************************/

#ifndef EventMainApp_h__
#define EventMainApp_h__

#include "EventNet.h"
#include "EventProcessor.h"

#include <list>
#include <string>


using namespace std;

typedef bool (* OnInitFunc)(void *arg, string &retName);
typedef void (* OnExitFunc)(void *arg);

class EventMain : public CommonThread, public EventProcessor
{
public:
    EventMain();
    virtual ~EventMain();

    virtual void    Run(int argc, char *argv[], int server_type);
    virtual void    Stop();

    virtual void    OnRegister() = 0;
    virtual bool    OnInit();
    virtual void    OnExit();
    virtual void    OnTimer();

 
    void            RegisterMoudle(OnInitFunc fpOninit, OnExitFunc fpOnExit);
    void            InitSignal();
    static void     sSignalProcess(int nSig);

    static void     sOnTimer(evutil_socket_t fd, short event, void *pData);
private:
    typedef list <OnInitFunc> INIT_FUNC_LIST;
    typedef list <OnExitFunc> EXIT_FUNC_LIST;

    INIT_FUNC_LIST  m_listInitFunc;
    EXIT_FUNC_LIST  m_listExitFunc;

    struct event    m_timerEv;
};

extern EventMain *g_pEventMain;
extern EventNet *g_pEventNet;

class EventSocket;
/*class EventNetPacket;*/
#include "EventPacket.h"
typedef struct MainThreadData
{
    EventSocket *m_pSocket;
    EventNetPacket *m_pMsg;

    MainThreadData()
    {
        m_pSocket = NULL;
        m_pMsg = NULL;
    }

    ~MainThreadData()
    {
        free(m_pMsg);
        m_pSocket = NULL;
        m_pMsg = NULL;
    }
}MTData;

#endif // EventMainApp_h__
