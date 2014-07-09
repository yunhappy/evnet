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
/*---------------------- 网络绘话对象 -----------------------*/

struct ISession
{
};

/*---------------------- 网络对象接口 -----------------------*/
struct INet 
{ 
    virtual int Send(const void * data,short len,const void * proto) = 0;
    virtual void Close() =0; 											//逻辑层线程驱动网络层将数据放入逻辑层
    virtual void Process(short nProcCount = 5) = 0;											/// 连接网络szIPAddr:"ip:port或者域名"
    virtual void Release() =0;
};

/*---------------------- 网络事件 -----------------------*/
struct INetEvent
{
    /// 连接成功事件
    virtual ISession* OnConnect(const char * szIP,short port){return 0;}
    /// 接受成功事件
    virtual ISession* OnAccept(INet * pNet,const char * szIP, short port){return 0;}
    /// 关闭事件
    virtual void OnClose(ISession * pNet,unsigned char bTimeout,unsigned char bNetConn,int err) =0;
    /// 接收数据事件
    virtual void OnReceive(ISession * pNet,const void * pData,short len,const void * proto) =0;
};

/*---------------------- 共享内存 -----------------------*/
struct IShareMemory 
{
    virtual void * GetShareMemory() = 0;
    virtual void Release() = 0;
};

/*---------------------- 网络引擎接口 -----------------------*/
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
