/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: PACKET_STRUCT_DB.h
* Summary: 

* Author: yunmiao
* Date: 2014/06/21  18:26
*******************************************************/

#ifndef PACKET_STRUCT_DB_h__
#define PACKET_STRUCT_DB_h__


#pragma pack(push,1)

struct DbMsgBase
{
    // 操作者id，线程分配id
    long long       m_i8OPGUID;
    // 操作方法
    virtual void    OnProcess(void *parma) {}

    DbMsgBase()
    {
        m_i8OPGUID = 0;
    }
};

#define DB_PACKET_STRUCT(type)  struct __##type
#define DB_OP_MSG(type)         struct __##type : public DbMsgBase

DB_OP_MSG(NET_MSG_TYPE_TEST)
{
    long long   m_nPlayerGUID;

    char        m_szUserName[128];
    char        m_szUserPws[128];
};

#pragma pack(pop)

#endif // PACKET_STRUCT_DB_h__