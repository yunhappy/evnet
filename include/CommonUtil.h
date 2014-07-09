/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: CommonUtil.h
* Summary

* Author: yunmiao
* Date: 2014-6-6 11:32
*******************************************************/

#ifndef CommonUtil_h__
#define CommonUtil_h__

#define GROUP_MOVE      (7*8)
#define SERVER_MOVE     (6*8)
#define COUNTER_MOVE    (4*8)
#define MAX_GUID_COUNT  (65535)

void InitGUIDParam(int nGroupIndex, int nServerIndex);

long long MakeGuid();

#endif // CommonUtil_h__