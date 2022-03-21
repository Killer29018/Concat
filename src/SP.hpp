#ifndef SP_HPP
#define SP_HPP

#include "Value.hpp"

class RC
{
private:
    uint32_t m_Count;
public:
    void addRef()
    {
        m_Count++;
    }

    uint32_t release()
    {
        return (--m_Count);
    }

    uint32_t getRefCount() { return m_Count; }
};

class SP
{
private:
    Value* m_Data;
    RC* m_Reference;
public:
    SP()
        : m_Data(new vNull())
    {
        m_Reference = new RC();
        m_Reference->addRef();
    }
    SP(Value* data)
        : m_Data(data)
    {
        m_Reference = new RC();
        m_Reference->addRef();
    }
    SP(const SP& sp) : m_Data(sp.m_Data), m_Reference(sp.m_Reference)
    {
        m_Reference->addRef();
    }

    ~SP()
    {
        if (m_Reference->release() == 0)
        {
            delete m_Data;
            delete m_Reference;
        }
    }

    uint32_t getRefCount() { return m_Reference->getRefCount(); }

    Value* get()
    {
        return m_Data;
    }

    Value& operator*()
    {
        return *m_Data;
    }

    Value* operator->()
    {
        return m_Data;
    }

    SP& operator=(const SP& sp)
    {
        if (this != &sp)
        {
            if (m_Reference->release() == 0)
            {
                delete m_Data;
                delete m_Reference;
            }

            m_Data = sp.m_Data;
            m_Reference = sp.m_Reference;
            m_Reference->addRef();
        }

        return *this;
    }
};

#endif
