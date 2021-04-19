#pragma once

#include "OleBow.hpp"

class MTypeAttr
{
public:
    MTypeAttr(MComPtr<ITypeInfo> i) : m_ta(NULL)
    {
        m_i = i;
        if (m_i)
            m_i->GetTypeAttr(&m_ta);
    }
    ~MTypeAttr()
    {
        if (m_ta)
        {
            m_i->ReleaseTypeAttr(m_ta);
            m_ta = NULL;
        }
    }
    TYPEATTR* operator->() const
    {
        return m_ta;
    }
protected:
    MComPtr<ITypeInfo> m_i;
    TYPEATTR* m_ta;
};
