#pragma once

#include "OleBow.hpp"

class MTypeLibAttr
{
public:
    MTypeLibAttr(MComPtr<ITypeLib> tl)
    {
        m_tl = tl;
        m_tl->GetLibAttr(&m_plibAttr);
    }
    ~MTypeLibAttr()
    {
        if (m_plibAttr)
        {
            m_tl->ReleaseTLibAttr(m_plibAttr);
            m_plibAttr = NULL;
        }
    }
    TLIBATTR* operator->() const
    {
        return m_plibAttr;
    }
protected:
    MComPtr<ITypeLib> m_tl;
    TLIBATTR* m_plibAttr;
};
