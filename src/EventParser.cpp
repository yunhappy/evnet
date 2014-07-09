/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: EventParser.cpp
* Summary

* Author: yunmiao
* Date: 2014-5-27 15:00
*******************************************************/


#include "EventParser.h"
#include "EventPacket.h"


EventParser::EventParser( EventSocket *pSocket )
{
    m_pEvSocket = pSocket;

    m_emStatus = PARSER_NONE;

    memset(&m_header, 0, sizeof(m_header));

    m_pCache = evbuffer_new();

    m_pBody = NULL;
    m_pData = NULL;
    m_pFunc = NULL;
}

EventParser::~EventParser()
{
    if (m_pCache)
    {
        evbuffer_free(m_pCache);
    }

    if (m_pBody)
    {
        evbuffer_free(m_pBody);
    }
}

PARSER_RESULT_EM EventParser::Parse()
{
    if (m_emStatus == PARSER_NONE)
    {
        m_emStatus = PARSEING_FIRST;
        memset(&m_header, 0, sizeof(m_header));
    }

    PARSER_RESULT_EM ret = PARSER_RET_ALL_READ;

    switch (m_emStatus)
    {
    case PARSER_NONE:
        break;
    case PARSEING_FIRST:
        ret = ReadFirst();
        break;
    case PARSEING_HEADERS:
        ret = ReadHeader();
        break;
    case PARSEING_BODYS:
        ret = ReadBody();
        break;
    default:
        break;
    }

    return ret;
}

void EventParser::OnReadComplete()
{
    size_t nHeadLen = sizeof(EventNetPacket);
    //nHeadLen -= 1;

    size_t nPackLen = nHeadLen + m_header.m_i2DataSize;

    EventNetPacket *pPacket = (EventNetPacket *)malloc(nPackLen);
    memset(pPacket, 0, nPackLen);
    memcpy(pPacket, &m_header, nHeadLen);
    
    evbuffer_remove(m_pBody, (void *)(pPacket->m_szData), m_header.m_i2DataSize);

    m_pFunc(m_pEvSocket, pPacket, m_pData);

    free(pPacket);

    //Çå³ý×´Ì¬£¬Test by pan
    m_emStatus = PARSER_NONE;
}

PARSER_RESULT_EM EventParser::ReadFirst()
{
    PARSER_RESULT_EM ret = ParserFirst();
    if (ret == PARSER_RET_NEED_MORE)
    {
        return PARSER_RET_NEED_MORE;
    }

    m_emStatus = PARSEING_HEADERS;
    return ReadHeader();
}

PARSER_RESULT_EM EventParser::ReadHeader()
{
    PARSER_RESULT_EM ret = ParserHeader();
    if (ret == PARSER_RET_NEED_MORE)
    {
        return PARSER_RET_NEED_MORE;
    }

    m_emStatus = PARSEING_BODYS;
    return ReadBody();
}

PARSER_RESULT_EM EventParser::ReadBody()
{
    PARSER_RESULT_EM ret = ParserBody();
    if (ret == PARSER_RET_NEED_MORE)
    {
        return PARSER_RET_NEED_MORE;
    }

    OnReadComplete();

    return PARSER_RET_ALL_READ;
}

PARSER_RESULT_EM EventParser::ParserFirst()
{
    do 
    {
        short i2First = 0;
        //printf("total recv %d\n", (int)evbuffer_get_length(m_pCache));
        if (evbuffer_get_length(m_pCache) < sizeof(i2First))
        {
            return PARSER_RET_NEED_MORE;
        }

        evbuffer_copyout(m_pCache, (void *)&i2First, sizeof(short));
        if (i2First == PACKET_HEAD)
        {
            break;
        }
        else
        {
            evbuffer_drain(m_pCache, 1);
        }
    } while (1);

    return PARSER_RET_ALL_READ;
}

PARSER_RESULT_EM EventParser::ParserHeader()
{
    size_t nHeadLen = sizeof(EventNetPacket);
    if (evbuffer_get_length(m_pCache) < nHeadLen)
    {
        return PARSER_RET_NEED_MORE;
    }

    evbuffer_remove(m_pCache, (void *)&m_header, nHeadLen);

    return PARSER_RET_ALL_READ;
}

PARSER_RESULT_EM EventParser::ParserBody()
{
    if (evbuffer_get_length(m_pCache) < size_t(m_header.m_i2DataSize))
    {
        return PARSER_RET_NEED_MORE;
    }

    if (m_pBody == NULL)
    {
        m_pBody = evbuffer_new();
    }
    
    evbuffer_remove_buffer(m_pCache, m_pBody, m_header.m_i2DataSize);

    
    return PARSER_RET_ALL_READ;
}
