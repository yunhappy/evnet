/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: Profiler.h
* Summary: 

* Author: yunmiao
* Date: 2014/07/08  15:00
*******************************************************/

#ifndef Profiler_h__
#define Profiler_h__

#include <stdio.h>
#include <time.h>

namespace common_profiler
{
    extern time_t m_nStartTime;

    extern int m_nPacketSendCount;
    extern int m_nPacketRecvCount;
    
    extern int m_nSendBytes;
    extern int m_nRecvBytes;

    void OnInit();

    void Log();
}


#endif // Profiler_h__
