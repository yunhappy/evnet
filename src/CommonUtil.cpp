/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: CommonUtil.cpp
* Summary

* Author: yunmiao
* Date: 2014-6-6 11:35
*******************************************************/

#include "CommonUtil.h"
#include <time.h>

unsigned short g_nCounters = 0;
long long g_ServerBase = 0;

void InitGUIDParam( int nGroupIndex, int nServerIndex )
{
    long long nTemp = 0;

    nTemp = nGroupIndex;
    g_ServerBase += nTemp << GROUP_MOVE;

    nTemp = nServerIndex;
    g_ServerBase += nTemp << SERVER_MOVE;
}

long long MakeGuid()
{
    long long nRes = 0;
    long long nTemp = 0;

    nRes += g_ServerBase;

    if (g_nCounters == MAX_GUID_COUNT)
    {
        g_nCounters = 0;
    }
    else
    {
        ++ g_nCounters;
    }

    nTemp = g_nCounters;
    nRes += nTemp << COUNTER_MOVE;

    int nTime = (int)time(NULL);

    nRes += nTime;

    return nRes;
}
