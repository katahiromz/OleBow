#pragma once

#include "OleBow.hpp"
#include "MElemDesc.hpp"

class MVarDesc
{
public:
    MVarDesc(MComPtr<ITypeInfo> i, int idx)
    {
        m_i = i;
        m_i->GetVarDesc(idx, &m_vardesc);
    }
    ~MVarDesc()
    {
        if (m_vardesc)
        {
            m_i->ReleaseVarDesc(m_vardesc);
            m_vardesc = NULL;
        }
    }
    _variant_t varValue()
    {
        return m_vardesc->lpvarValue;
    }
    VARDESC* operator->() const
    {
        return m_vardesc;
    }
    MElemDesc& elemDescVar()
    {
        return reinterpret_cast<MElemDesc&>(m_vardesc->elemdescVar);
    }
protected:
    MComPtr<ITypeInfo> m_i;
    VARDESC* m_vardesc;
};
