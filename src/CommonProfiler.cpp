/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: Profiler.cpp
* Summary: 

* Author: yunmiao
* Date: 2014/07/08  15:02
*******************************************************/

#include "CommonProfiler.h"

namespace common_profiler
{
    time_t m_nStartTime = 0;

    int m_nPacketSendCount = 0;
    int m_nPacketRecvCount = 0;

    int m_nSendBytes = 0;
    int m_nRecvBytes = 0;

    void OnInit()
    {
        m_nStartTime = time(NULL);
    }

    void Log()
    {
        time_t now = time(NULL);
        time_t tick = now - m_nStartTime;

        printf("t[%d] s[%d] r[%d] [%d][%d]b/s \n", (int)tick, 
            (int)(m_nPacketSendCount/tick) , (int)(m_nPacketRecvCount/tick), (int)(m_nSendBytes/tick), (int)(m_nRecvBytes/tick));
    }

}
