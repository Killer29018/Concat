#ifndef SMART_POINTER_HPP
#define SMART_POINTER_HPP

#include "Value.hpp"

class RefCount
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

class SmartPointer
{
private:
    Value* m_Data;
    RefCount* m_Reference;
public:
    SmartPointer()
        : m_Data(new Value())
    {
        m_Reference = new RefCount();
        m_Reference->addRef();
    }
    SmartPointer(Value* data)
        : m_Data(data)
    {
        m_Reference = new RefCount();
        m_Reference->addRef();
    }
    SmartPointer(const SmartPointer& sp) : m_Data(sp.m_Data), m_Reference(sp.m_Reference)
    {
        m_Reference->addRef();
    }

    ~SmartPointer()
    {
        if (m_Reference->release() == 0)
        {
            delete m_Data;
            delete m_Reference;
        }
    }

    uint32_t getRefCount() { return m_Reference->getRefCount(); }

    Value* get() { return m_Data; }
    const Value* get() const { return m_Data; }

    RefCount* getRef() { return m_Reference; }
    const RefCount* getRef() const { return m_Reference; }


    Value& operator*() { return *m_Data; }
    const Value& operator*() const { return *m_Data; }

    Value* operator->() { return m_Data; }
    const Value* operator->() const { return m_Data; }

    bool operator!() const { return !m_Data; }

    SmartPointer& operator=(const SmartPointer& sp)
    {
        if (this == &sp)
            return *this;

        if (m_Reference->release() == 0)
        {
            delete m_Data;
            delete m_Reference;
        }

        m_Data = sp.m_Data;
        m_Reference = sp.m_Reference;
        m_Reference->addRef();

        return *this;
    }

    // template<class Y>
    // SmartPointer<Y>& operator=(const SmartPointer<Y>&& sp)
    // {
    //     if (m_Reference->release() == 0)
    //     {
    //         delete m_Data;
    //         delete m_Reference;
    //     }

    //     m_Data = sp.m_Data;
    //     m_Reference = sp.m_Reference;
    //     m_Reference->addRef();

    //     return *this;
    // }
};

template <typename T, typename... TArgs>
SmartPointer makeSmartPointer(TArgs... args) { return SmartPointer(new T(args...)); }

template <typename T>
SmartPointer makeSmartPointer() { return SmartPointer(); }

#endif
