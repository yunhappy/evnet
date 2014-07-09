/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventMain.cpp
* Summary

* Author: yunmiao
* Date: 2014-6-9 15:55
*******************************************************/

#include "EventMain.h"
#include <signal.h>
#include "EventPacket.h"
#include <assert.h>
#include "Timer.h"
#include "CommonProfiler.h"

int g_nSvrID;
int g_nSvrType;

EventMain *g_pEventMain = NULL;
EventNet *g_pEventNet = NULL;

EventMain::EventMain()
{
    if (g_pEventMain != NULL)
    {
        assert(0);
    }

    g_pEventMain = this;
}

EventMain::~EventMain()
{

}

void EventMain::Run( int argc, char *argv[], int server_type )
{
    if (!EventNet::Init())
    {
        printf("WTF, Error !\n");
        return;
    }

    EventNetMsg::sInit();


    if (argc != 2)
    {
        return;
    }

    g_nSvrID = atoi(argv[1]);

    g_nSvrType = server_type;

    // 随机种子初始化

    // 信号处理初始化
    InitSignal();

    // 初始化系统modoule
    OnRegister();

    if (!OnInit())
    {
        return;
    }

    // 初始化timer
    {
        const int fps = 1;

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = (1000/fps) * 1000;

        event_assign(&m_timerEv, g_pEvMainBase, -1, EV_PERSIST, &sOnTimer, (void *)this);

        event_add(&m_timerEv, &tv);
    }

    common_profiler::OnInit();

    // Net消息循环
    EventNet netBase;
    g_pEventNet = &netBase;

    netBase.Start();


    // 游戏主循环
    while (1)
    {
        EventNet::MainLoop();

        void *val = NULL;
        if(!g_pEventMain->TryPop(val))
        {
            mars_time::sleep(1);
            continue;
        }

        // msg process;TODO FREE
        MainThreadData *pTData = (MainThreadData *)val;
        OnNewMsg((enum NET_MSG_TYPE)(pTData->m_pMsg->m_i2PacketID), pTData);

        delete pTData;
    }
    
    // 游戏主循环

    EventNet::Exit();

    OnExit();

    return;
}

void EventMain::Stop()
{

}


bool EventMain::OnInit()
{
    INIT_FUNC_LIST::iterator iter = m_listInitFunc.begin();
    for (int n = 0; iter != m_listInitFunc.end(); ++ iter)
    {
        OnInitFunc pFn = *iter;
        if (pFn)
        {
            string name;
            bool ret = (*pFn)(NULL, name);
            if (!ret)
            {
                //char buf[512] = {0};
                // log

		printf("****%d***\n", n);
                return false;
            }
        }
    }

    return true;
}

void EventMain::OnExit()
{
    EXIT_FUNC_LIST::reverse_iterator rIter = m_listExitFunc.rbegin();
    for (; rIter != m_listExitFunc.rend(); ++ rIter)
    {
        OnExitFunc pFn = *rIter;
        (*pFn)(NULL);
    }
    m_listExitFunc.clear();

    return;
}

void EventMain::OnTimer()
{

}

void EventMain::RegisterMoudle( OnInitFunc fpOninit, OnExitFunc fpOnExit )
{
    if (fpOninit)
    {
        m_listInitFunc.push_back(fpOninit);
    }

    if (fpOnExit)
    {
        m_listExitFunc.push_back(fpOnExit);
    }
}


void EventMain::InitSignal()
{
    signal(SIGINT, sSignalProcess);
    signal(SIGILL, sSignalProcess);
    signal(SIGTERM, sSignalProcess);

#ifdef _WIN32
//     if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, true ))
//     {
//         // log
//         fprintf(stderr, "Unable install handler \n");
//     }
#else
    signal(SIGXFSZ, sSignalProcess);
    signal(SIGUSR1, sSignalProcess);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
#endif
}


void EventMain::sSignalProcess(int nSig)
{
    // 日子输出完
    printf("signal [%d]\n", nSig);

    switch (nSig)
    {
    case SIGINT:
        // call stack
        break;
    case SIGILL:
        break;
    case SIGTERM:
        // user stop server
        break;
#ifndef _WIN32
    case SIGXFSZ:
        break;
    case SIGUSR1:
        // user spe signal
        break;
#endif
    default:
        break;
    }

    exit(0);
}

void EventMain::sOnTimer(evutil_socket_t fd, short event, void *pData)
{
    g_pEventMain->OnTimer();
}
