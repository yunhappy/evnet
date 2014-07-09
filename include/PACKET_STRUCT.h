/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: PACKET_STRUCT.h
* Summary: 

* Author: yunmiao
* Date: 2014/06/15  18:10
*******************************************************/

#ifndef PacketStruct_h__
#define PacketStruct_h__

#define NET_MSG_STRUCT_DEF(type)                        struct _##type

#pragma pack(push,1)


//====服务器内部协议====================================================

NET_MSG_STRUCT_DEF(HAND_SHAKE)
{
    int     m_nFlag;
};

NET_MSG_STRUCT_DEF(ROBOT_TEST_PACKET)
{
    long long   m_i8Time;
    int         m_nLen;
    char        m_nData[0];
};

//====================================================================

NET_MSG_STRUCT_DEF(CL_CONN_SVR)
{
    char        m_szAccountID[32];  //--玩家ID
    char        m_szSign[32];       //--玩家会话ID
    int         m_nTime;            //
    int         m_nAdultFlag;       //--防沉迷标记
    int         m_nFirstLogin;  
    char        m_szUF[32];         //--渠道
};

NET_MSG_STRUCT_DEF(LC_CONN_RESP)
{
    int         m_nResultCode;  //0:Error 1::OK 2:Create Role
    char        m_szAccountID[32];
    int         m_nRoleCount;
    int         m_nAvoidTime;
    int         m_nAvoidValue;
    int         m_nClearTime;
};

NET_MSG_STRUCT_DEF(CL_CREATE_ROLE_REQ)
{
    char    m_szRoleName[32];   //--角色名字
    int     m_nRoleSex;         //--角色性别
    int     m_nFace;
    int     m_nHair;
    int     m_nSuitID;      //--套装ID
    int     m_nRoleProf;    //--角色职业
    int     m_nIncoID;      //--玩家头像
    char    m_szUF[32];     //--渠道
};

NET_MSG_STRUCT_DEF(LC_CREATE_ROLE_RESP)
{
    int     m_nResultCode;
    int     m_nRoleID;
    char    m_szRoleName[32];
};

#pragma pack(pop)

#endif // PacketStruct_h__