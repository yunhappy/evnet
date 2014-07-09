/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventNet.h
* Summary

* Author: yunmiao
* Date: 2014-5-23 11:44
*******************************************************/

#ifndef EventNet_h__
#define EventNet_h__


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#endif

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/thread.h>

#include <string>
#include <map>

#include "CommonThread.h"

using namespace std;

class EventNet : public CommonThread
{
public:
	EventNet();
	~EventNet();

    static bool     Init();
    static void     NetLoop();
    static void     MainLoop();
    static void     Exit();
   
    virtual void    Run();
};



extern struct event_base *g_pEvNetBase;
extern struct event_base *g_pEvMainBase;

#endif // EventNet_h__