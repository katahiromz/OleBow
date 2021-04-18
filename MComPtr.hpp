#pragma once

#include <cassert>

template <typename T>
class MComPtr
{
public:
    T* m_p;
    MComPtr() : m_p(NULL)
    {
    }
    MComPtr(T *p)
    {
        m_p = p;
        if (m_p)
            m_p->AddRef();
    }
    MComPtr(const MComPtr<T>& p)
    {
        m_p = p.m_p;
        if (m_p)
            m_p->AddRef();
    }
    ~MComPtr()
    {
        if (m_p)
            m_p->Release();
    }

    bool operator==(const MComPtr<T>& p) const
    {
        return m_p == p.m_p;
    }
    bool operator!=(const MComPtr<T>& p) const
    {
        return m_p != p.m_p;
    }
    bool operator<(const MComPtr<T>& p) const
    {
        return m_p < p.m_p;
    }
    bool operator>(const MComPtr<T>& p) const
    {
        return m_p > p.m_p;
    }
    bool operator<=(const MComPtr<T>& p) const
    {
        return m_p <= p.m_p;
    }
    bool operator>=(const MComPtr<T>& p) const
    {
        return m_p >= p.m_p;
    }
    T** operator&()
    {
        assert(m_p == NULL);
        return &m_p;
    }
    operator T*()
    {
        return m_p;
    }
    T* operator->()
    {
        assert(m_p);
        return m_p;
    }
    T* operator=(T* p)
    {
        T* old = m_p;
        m_p = p;
        if (m_p)
            m_p->AddRef();
        if (old)
            old->Release();
        return *this;
    }
    T* operator=(const MComPtr<T>& p)
    {
        T* old = m_p;
        m_p = p.m_p;
        if (m_p)
            m_p->AddRef();
        if (old)
            old->Release();
        return *this;
    }

    void Attach(T* p)
    {
        if (m_p)
            m_p->Release();
        m_p = p;
    }
    T* Detach()
    {
        T* old = m_p;
        m_p = NULL;
        return old;
    }
    void Release()
    {
        if (m_p)
        {
            m_p->Release();
            m_p = NULL;
        }
    }
};
