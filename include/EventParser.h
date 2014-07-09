/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventParser.h
* Summary

* Author: yunmiao
* Date: 2014-5-27 14:33
*******************************************************/

#ifndef EventParser_h__
#define EventParser_h__

#include "EventSocket.h"
#include "EventPacket.h"

enum PARSER_STATUS_EM
{
    PARSER_NONE = 0,
    PARSEING_FIRST = 1,
    PARSEING_HEADERS = 2,
    PARSEING_BODYS = 3,
};

enum PARSER_RESULT_EM
{
    PARSER_RET_ALL_READ = 0,
    PARSER_RET_NEED_MORE = 1,
};

typedef void (*fpParserOnRead)(EventSocket *pSocket, EventNetPacket *pMsg, void *pData);

class EventParser
{
public:
	EventParser(EventSocket *pSocket);
	~EventParser();

    PARSER_RESULT_EM    Parse();

    void                OnReadComplete();

    void                SetCallBack(fpParserOnRead fp, void *pData) { m_pFunc = fp; m_pData = pData; }

    evbuffer            *GetCache() const       { return m_pCache; }

    EventNetPacket*     GetNetPacket()   {return &m_header;}

protected:
	PARSER_RESULT_EM    ReadFirst();
    PARSER_RESULT_EM    ReadHeader();
    PARSER_RESULT_EM    ReadBody();

    PARSER_RESULT_EM    ParserFirst();
    PARSER_RESULT_EM    ParserHeader();
    PARSER_RESULT_EM    ParserBody();

private:
    PARSER_STATUS_EM    m_emStatus;

    evbuffer            *m_pBody;
    evbuffer            *m_pCache;

    // ref
    EventSocket         *m_pEvSocket;


    void                *m_pData;
    fpParserOnRead      m_pFunc;

    EventNetPacket      m_header;
};


class ParserStateBase
{
public:
    ParserStateBase();
    virtual ~ParserStateBase();


    virtual void StateLogic();
};

class ParserFirstState : public ParserStateBase
{
public:
    ParserFirstState();
    virtual ~ParserFirstState();


    virtual void StateLogic();
};


#endif // EventParser_h__