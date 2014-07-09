/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: INetworklib.h
* Summary: 

* Author: yunmiao
* Date: 2014/07/09  11:15
*******************************************************/

#ifndef INetworklib_h__
#define INetworklib_h__
/*---------------------- ����滰���� -----------------------*/

struct ISession
{
};

/*---------------------- �������ӿ� -----------------------*/
struct INet 
{ 
    virtual int Send(const void * data,short len,const void * proto) = 0;
    virtual void Close() =0; 											//�߼����߳���������㽫���ݷ����߼���
    virtual void Process(short nProcCount = 5) = 0;											/// ��������szIPAddr:"ip:port��������"
    virtual void Release() =0;
};

/*---------------------- �����¼� -----------------------*/
struct INetEvent
{
    /// ���ӳɹ��¼�
    virtual ISession* OnConnect(const char * szIP,short port){return 0;}
    /// ���ܳɹ��¼�
    virtual ISession* OnAccept(INet * pNet,const char * szIP, short port){return 0;}
    /// �ر��¼�
    virtual void OnClose(ISession * pNet,unsigned char bTimeout,unsigned char bNetConn,int err) =0;
    /// ���������¼�
    virtual void OnReceive(ISession * pNet,const void * pData,short len,const void * proto) =0;
};

/*---------------------- �����ڴ� -----------------------*/
struct IShareMemory 
{
    virtual void * GetShareMemory() = 0;
    virtual void Release() = 0;
};

/*---------------------- ��������ӿ� -----------------------*/
typedef void (*NetLogCallbackFunc)(const char * szInfo);
struct INetworkLib
{		
    virtual INet * Listen(const char * szIPAddr,INetEvent * pEvent,int nMaxCount,int nMaxMsgSize,int nCacheSize = 0 ,int nType = 0) = 0;
    virtual INet * Connect(const char * szIPAddr,INetEvent * pEvent,int nMaxMsgSize,int nCacheSize = 0,int nType = 0) =0;
    virtual void SetLogCallback(NetLogCallbackFunc func) = 0;
    virtual void Release() =0; 
}; 

namespace libNet
{
    INetworkLib * NetworkLibCreate(bool bOpenThread);
}
#endif // INetworklib_h__
