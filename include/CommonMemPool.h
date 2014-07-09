/*******************************************************
* Copyright (c) 2014, yunhappy All rights reserved.
* 
* FileName: CommonMemPool.h
* Summary

* Author: yunmiao
* Date: 2014-6-6 11:03
*******************************************************/

#ifndef EventMemPool_h__
#define EventMemPool_h__

#include <stack>

using namespace std;

class IObjMemPool
{
public:
    IObjMemPool() { m_nUseNum = 0; m_nAssignNum = 0; };
    virtual ~IObjMemPool() {};

	virtual bool Init(int nMin, int nMax, int nIncrese) = 0;
    virtual bool Alloc(int nNum) = 0;
    virtual void *Pop() = 0;
    virtual void Push(void *p) = 0;
    virtual void Clear() = 0;

    int m_nUseNum;
    int m_nAssignNum;
    int m_nMinNum;
    int m_nMaxNum;
    int m_nIncreseNum;
};

template<typename T>
class SafeObjMemPool : public IObjMemPool
{
public:
    SafeObjMemPool() {};
    virtual ~SafeObjMemPool() {};

    virtual bool Init(int nMin, int nMax, int nIncrese)
    {
        m_nMinNum = nMin;
        m_nMaxNum = nMax;
        m_nIncreseNum = nIncrese;

        return Alloc(nMin);
    }
	
    virtual bool Alloc(int nNum)
    {
        if (m_nAssignNum >= m_nMaxNum)
        {
            m_nMaxNum *= 2;
        }

        for (int i = 0; i < nNum; ++ i)
        {
            T *p = (T*)malloc(sizeof(T));
            new (p) T;
            if (p != NULL)
            {
                ((T*)p)->SetDirty(false);
                ((T*)p)->SetPool(this);

                m_objStack.push(p);

                ++ m_nAssignNum;
            }
            else
            {
                //logo
                return false;
            }
        }

        return true;
    }

    virtual void *Pop()
    {
        if (m_objStack.size() > 0)
        {
            void *p = m_objStack.top();
            m_objStack.pop();

            ((T*)p)->SetDirty(false);

            ++ m_nUseNum;

            return p;
        }
        else
        {
            if (Alloc(m_nIncreseNum))
            {
                if (m_objStack.size() > 0)
                {
                    void *p = m_objStack.top();
                    m_objStack.pop();

                    ((T*)p)->SetDirty(false);

                    ++ m_nUseNum;

                    return p;
                }
            }
        }

        return NULL;
    }

    virtual void Push(void *p)
    {
        if (((T*)p)->GetDirty())
        {
            //todo log
            assert(0);
            return;
        }

        if (((T*)p)->GetPool() != this)
        {
            assert(0);
            return;
        }

        ((T*)p)->SetDirty(true);
        ((T*)p)->Clear();

        m_objStack.push((T*)p);
        -- m_nUseNum;
    }

    virtual void Clear()
    {
        while (!m_objStack.empty())
        {
            T *p = m_objStack.top();
            if (p)
            {
                p->~T();
                free(p);
            }
            m_objStack.pop();
        }
    }

protected:
    typedef stack<T *>      OBJ_STACK;
    OBJ_STACK               m_objStack;
};

class ISafePoolObj
{
public:
	ISafePoolObj() : m_bDirty(false), m_pool(NULL) {}
	virtual ~ISafePoolObj() {}

    bool GetDirty() const               { return m_bDirty; }
    void SetDirty(bool val)             { m_bDirty = val; }

    void * GetPool() const              { return m_pool; }
    void SetPool(void * val)            { m_pool = val; }

private:
    bool m_bDirty;
    void *m_pool;
};

#endif // EventMemPool_h__