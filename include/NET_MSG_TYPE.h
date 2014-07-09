/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: NET_MSG_TYPE.h
* Summary: 

* Author: yunmiao
* Date: 2014/06/21  18:14
*******************************************************/

#ifndef NET_MSG_TYPE_h__
#define NET_MSG_TYPE_h__



enum NET_MSG_TYPE
{
    MSG_TYPE_MIN = 0,
#define _NET_MSG_DEF(type, id)  type = id,
#include "INNER_NET_MSG.h"

    NET_MSG_TYPE_MIN = 500,
#define _NET_MSG_DEF(type, id) type = id,
#include "DB_NET_MSG.h"
    NET_MSG_TYPE_MAX,

//------Client To Login--------
    CL_MSG_TYPE_MIN = 1000,
#define _NET_MSG_DEF(type, id)  type = id,
#include "CL_NET_MSG.h"
    CL_MSG_TYPE_MAX, 

//------Client To World--------
    CW_MSG_TYPE_MIN = 2000,
#define _NET_MSG_DEF(type, id)  type = id,
#include "CW_NET_MSG.h"
    CW_MSG_TYPE_MAX,

//------Client To Scene--------
    CS_MSG_TYPE_MIN = 3000,
#define _NET_MSG_DEF(type, id)  type = id,
#include "CS_NET_MSG.h"
    CS_MSG_TYPE_MAX,  

//------Conn To Client--------
    SC_MSG_MIN = 6000,
#define _NET_MSG_DEF(type, id)  type = id,
#include "SC_NET_MSG.h"
    SC_MSG_TYPE_MAX,

    MSG_TYPE_MAX
};

#endif // NET_MSG_TYPE_h__
