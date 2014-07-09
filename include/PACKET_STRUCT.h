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


//====�������ڲ�Э��====================================================

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
    char        m_szAccountID[32];  //--���ID
    char        m_szSign[32];       //--��һỰID
    int         m_nTime;            //
    int         m_nAdultFlag;       //--�����Ա��
    int         m_nFirstLogin;  
    char        m_szUF[32];         //--����
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
    char    m_szRoleName[32];   //--��ɫ����
    int     m_nRoleSex;         //--��ɫ�Ա�
    int     m_nFace;
    int     m_nHair;
    int     m_nSuitID;      //--��װID
    int     m_nRoleProf;    //--��ɫְҵ
    int     m_nIncoID;      //--���ͷ��
    char    m_szUF[32];     //--����
};

NET_MSG_STRUCT_DEF(LC_CREATE_ROLE_RESP)
{
    int     m_nResultCode;
    int     m_nRoleID;
    char    m_szRoleName[32];
};

#pragma pack(pop)

#endif // PacketStruct_h__