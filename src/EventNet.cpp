/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventNet.cpp
* Summary

* Author: yunmiao
* Date: 2014-5-23 11:45
*******************************************************/

#include "EventNet.h"
#include "EventMain.h"
#include "EventSocket.h"
#include "Timer.h"

//net base
struct event_base *g_pEvNetBase = NULL;

//main base
struct event_base *g_pEvMainBase = NULL;

EventNet::EventNet()
{
    
}

EventNet::~EventNet()
{

}

bool EventNet::Init()
{
    struct event_config *cfg = event_config_new();

#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);

#else

    if (event_config_require_features(cfg, EV_FEATURE_ET) == -1)
    {
        printf("**************EV_FEATURE_ET error**************n");
    }

    event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);

#ifdef EVNET_USE_THREAD
    evthread_use_pthreads();
#endif

#endif

    g_pEvNetBase = event_base_new_with_config(cfg);
    if (g_pEvNetBase == NULL)
    {
        exit(1);
    }

    g_pEvMainBase = event_base_new_with_config(cfg);
    if (g_pEvMainBase == NULL)
    {
        exit(1);
    }

    return true;
}

void EventNet::NetLoop()
{
    //event_base_dispatch(g_pEvNetBase);
    event_base_loop(g_pEvNetBase, EVLOOP_NONBLOCK);
}

void EventNet::MainLoop()
{
    //event_base_dispatch(g_pEvMainBase);
    event_base_loop(g_pEvMainBase, EVLOOP_NONBLOCK);
}

void EventNet::Exit()
{
    event_base_free(g_pEvNetBase);
    event_base_free(g_pEvMainBase);
}

void EventNet::Run()
{
    // TODO ÑÓ³Ù ÍøÂçÑ­»·
    while (1)
    {
        EventNet::NetLoop();

        void *val = NULL;
        if(!g_pEventNet->TryPop(val))
        {
            mars_time::sleep(1);
            continue;
        }

        // msg process; TODO FREE
        MainThreadData *pTData = (MainThreadData *)val;
        (pTData->m_pSocket)->SendNetMsg(pTData->m_pMsg);

        delete pTData;
    }

    EventNet::NetLoop();
}


