#pragma once

#include "OleBow.hpp"

class MFuncDesc
{
public:
    MFuncDesc(MComPtr<ITypeInfo> i, int idx)
    {
        m_i = i;
        m_i->GetFuncDesc(idx, &m_funcdesc);
    }
    ~MFuncDesc()
    {
        if (m_funcdesc)
        {
            m_i->ReleaseFuncDesc(m_funcdesc);
            m_funcdesc = NULL;
        }
    }
    FUNCDESC* operator->() const
    {
        return m_funcdesc;
    }
    Ptr<List<ELEMDESC*> > elemdescParams()
    {
        auto ret = MakePtr<List<ELEMDESC*> >();
        for (int x = 0; x < m_funcdesc->cParams; ++x)
        {
            ret->push_back(&m_funcdesc->lprgelemdescParam[x]);
        }
        return ret;
    }
protected:
    MComPtr<ITypeInfo> m_i;
    FUNCDESC* m_funcdesc;
};
