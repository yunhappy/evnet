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

//网络包统计;
struct PacketStat
{
    int     m_nRecvNum;         // 接收包数量;
    int     m_nSendNum;         // 发送包数量;
    int     m_nRecvByte;
    int     m_nSendByte;
    int     m_MsgIDArr[MSG_TYPE_MAX];   // 统计各包数量;
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

// 用户在Conn上的基础信息;
struct RoleConnInfo
{
    RoleConnStatus m_nStatus;   //玩家状态
    EventSocket* m_pSocket;     //socket链接
   
    RoleConnInfo()
    {
        m_nStatus = ConnNone;
        m_pSocket = NULL;
    }
};


struct RoleShowInfo
{
    char    m_szRoleName[32];
    int     m_nRoleSex;         //性别     
    int     m_nProf;            //职业
    int     m_nIconID;

    int     m_nFace;
    int     m_nHair;
    int     m_nDress;           //--衣服
    int     m_nHand;            //--护手
    int     m_nShoe;            //--鞋子 
    int     m_nArms;            //--武器
    int     m_nHoresID;         //--坐骑
    //等级
    int     m_nDressLv;           //--衣服
    int     m_nHandLv;            //--护手
    int     m_nShoeLv;            //--鞋子 
    int     m_nArmsLv;            //--武器
    //默认
    int     m_nDefFace;
    int     m_nDefHair;
    int     m_nDefDress;           //--衣服
    int     m_nDefHand;            //--护手
    int     m_nDefShoe;            //--鞋子 
    int     m_nDefArms;            //--武器
 
    int     m_nExp;                 //--总经验
    int     m_nLevel;               //--等级
    int     m_nLvMaxExp;        
    int     m_nLvCurExp;
    int     m_nNewRoleFlag;
};


#endif // CommonStruct_h__


