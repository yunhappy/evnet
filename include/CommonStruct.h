/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: CommonStruct.h
* Summary: 

* Author: pan
* Date: 2014/06/12  17:09
*******************************************************/

#ifndef CommonStruct_h__
#define CommonStruct_h__

#include "CommonEnum.h"
#include "EventSocket.h"
#include "NET_MSG_TYPE.h"
#include "Logger.h"

//�����ͳ��;
struct PacketStat
{
    int     m_nRecvNum;         // ���հ�����;
    int     m_nSendNum;         // ���Ͱ�����;
    int     m_nRecvByte;
    int     m_nSendByte;
    int     m_MsgIDArr[MSG_TYPE_MAX];   // ͳ�Ƹ�������;
    void Clear()
    {
        m_nRecvNum = 0;
        m_nSendNum = 0;
        m_nRecvByte = 0;
        m_nSendByte = 0;
        memset(m_MsgIDArr, 0, sizeof(m_MsgIDArr));
    }
    void Print(bool bDetail = false)
    {
        gSystemStream("RecvNum: " << m_nRecvNum << "; RecvBytes: " << m_nRecvByte << \
            "; SendNum: " << m_nSendNum << "; SendBytes: " << m_nSendByte);
        if (bDetail)
        {
            for(int i = 0 ; i < MSG_TYPE_MAX ; i ++)
            {
                if (m_MsgIDArr[i] > 0)
                {
                    gSystemStream("MsgID[" << i << "] Num: " << m_MsgIDArr[i]);
                }
            }
        }
    }
};

// �û���Conn�ϵĻ�����Ϣ;
struct RoleConnInfo
{
    RoleConnStatus m_nStatus;   //���״̬
    EventSocket* m_pSocket;     //socket����
   
    RoleConnInfo()
    {
        m_nStatus = ConnNone;
        m_pSocket = NULL;
    }
};


struct RoleShowInfo
{
    char    m_szRoleName[32];
    int     m_nRoleSex;         //�Ա�     
    int     m_nProf;            //ְҵ
    int     m_nIconID;

    int     m_nFace;
    int     m_nHair;
    int     m_nDress;           //--�·�
    int     m_nHand;            //--����
    int     m_nShoe;            //--Ь�� 
    int     m_nArms;            //--����
    int     m_nHoresID;         //--����
    //�ȼ�
    int     m_nDressLv;           //--�·�
    int     m_nHandLv;            //--����
    int     m_nShoeLv;            //--Ь�� 
    int     m_nArmsLv;            //--����
    //Ĭ��
    int     m_nDefFace;
    int     m_nDefHair;
    int     m_nDefDress;           //--�·�
    int     m_nDefHand;            //--����
    int     m_nDefShoe;            //--Ь�� 
    int     m_nDefArms;            //--����
 
    int     m_nExp;                 //--�ܾ���
    int     m_nLevel;               //--�ȼ�
    int     m_nLvMaxExp;        
    int     m_nLvCurExp;
    int     m_nNewRoleFlag;
};


#endif // CommonStruct_h__


