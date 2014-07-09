/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: CommonMacros.h
* Summary

* Author: yunmiao
* Date: 2014-5-23 14:49
*******************************************************/

#ifndef CommonMacros_h__
#define CommonMacros_h__

#include <string>

using namespace std;

#define SAFE_DELETE(p)              { if(p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p)        { if(p) { delete[] (p); (p) = NULL; } }
#define SAFE_RELEASE(p)             { if(p) { (p)->Release(); (p) = NULL; } }

#define NET_MSG_STRUCT_DEF(type)                        struct _##type

#define REG_DEFAULT_PROCESS_HANDLE(fpmap, type)         fpmap[type] = Process_##type;   
#define DEF_MSG_HANDLE(type)                            Process_##type(EventSocket *pSocket, EventNetPacket *pMsg)
#define S_DEFAULT_PROCESS_HANDLE_INIT(type)               static void DEF_MSG_HANDLE(type)
#define DEFAULT_PROCESS_HANDLE_INIT(type)               void DEF_MSG_HANDLE(type)

#define _G_INIT_FUNC(type)                      _##type##_OnInit
#define _G_EXIT_FUNC(type)                      _##type##_OnExit

#define G_MOUDLE_REGISTER(type)                 RegisterMoudle(_G_INIT_FUNC(type), _G_EXIT_FUNC(type));

#define G_MOUDLE_DEFINITION(type, moudlePoint)          type * moudlePoint = NULL;\
                                                        bool _G_INIT_FUNC(type)(void *arg, string &name) {\
                                                            name = #type; \
                                                            if (moudlePoint != NULL) return false; \
                                                            moudlePoint = new type;\
                                                            printf("MOUDLE INIT "#type"\n");\
                                                            return moudlePoint->_OnInit();\
                                                        }\
                                                        void _G_EXIT_FUNC(type)(void *arg) {\
                                                            printf("MOUDLE EXIT "#type"\n");\
                                                            moudlePoint->_OnExit();\
                                                            SAFE_DELETE(moudlePoint);\
                                                        }

#define G_MOUDLE_EXTERN(type, moudlePoint)              extern type * moudlePoint;\
                                                        bool _G_INIT_FUNC(type)(void *arg, string &name);\
                                                        void _G_EXIT_FUNC(type)(void *arg);

#define MSG_HANDLE_DEFINITION(type, pServer) _##type *pPack = (_##type *)pMsg->m_szData;\
    EventNetMsg Msg;\
    if (Msg.InitFromMsgType(type)){\
    _##type *pStruct = (_##type *)Msg.GetBody();\
    memcpy(pStruct, pPack, sizeof(_##type));\
    Msg.m_pSrcSocket = pSocket;\
    Msg.SendTo(pServer);\
    }\

#endif // CommonMacros_h__